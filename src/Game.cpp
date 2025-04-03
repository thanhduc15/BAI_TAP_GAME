#include "Game.h"

Game::Game() :
    player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - TILE_SIZE),
    playerTankImage(nullptr),
    enemyTankImage(nullptr),
    bulletImage(nullptr),
    wallImage(nullptr),
    healthImage(nullptr),
    background(nullptr),
    menu(nullptr),
    amThanh(AmThanh::getInstance()),
    running(true),
    lastEnemySpawnTime(0),
    lastHealthSpawnTime(0),
    gameState(MENU_SCREEN),
    highScore(0) { // Khoi tao highScore

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    } else {
        std::cout << "SDL_ttf initialized successfully!" << std::endl;
    }

    if (!amThanh.init()) {
        std::cerr << "Failed to initialize sound system!" << std::endl;
    }

    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Battle City Galaxy", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (loadResources()) {
        player.setImage(playerTankImage);
        for (auto& wall : walls) {
            wall.setImage(wallImage);
        }
    }

    amThanh.loadSound("ban", "assets/ban.wav");
    amThanh.loadSound("no", "assets/no.wav");
    amThanh.loadSound("trung_dan", "assets/trung_dan.wav");
    amThanh.loadMusic("assets/nhac_nen.mp3");

    amThanh.setMusicVolume(50);
    amThanh.setSoundVolume("ban", 70);
    amThanh.setSoundVolume("no", 80);
    amThanh.setSoundVolume("trung_dan", 60);

    amThanh.playMusic();

    background = new Background(renderer, "assets/galaxy_bg_vertical.png");
    background->setScrollSpeed(2);

    menu = new Menu(renderer);
    menu->loadFont();

    generateWalls();
    generateEnemies();
    loadHighScore();
}

Game::~Game() {
    delete playerTankImage;
    delete enemyTankImage;
    delete bulletImage;
    delete wallImage;
    delete healthImage;
    delete background;
    delete menu;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool Game::loadResources() {
    playerTankImage = new Image(renderer, "assets/player_tank.png");
    enemyTankImage = new Image(renderer, "assets/enemy_tank.png");
    bulletImage = new Image(renderer, "assets/bullet.png");
    wallImage = new Image(renderer, "assets/wall.png");
    healthImage = new Image(renderer, "assets/health_pack.png");

    if (!playerTankImage->getTexture() || !enemyTankImage->getTexture() ||
        !bulletImage->getTexture() || !wallImage->getTexture() ||
        !healthImage->getTexture()) {
        std::cerr << "Failed to load one or more images!" << std::endl;
        return false;
    }

    return true;
}

// Tao cac tuong bao ve duoi man hinh
void Game::generateWalls() {
    walls.clear();
    for (int i = 0; i < SCREEN_WIDTH; i += TILE_SIZE) {
        walls.push_back(Wall(i, SCREEN_HEIGHT - TILE_SIZE));
        walls.back().setImage(wallImage);
    }
}

// Tao ke dich ban dau
void Game::generateEnemies() {
    enemies.clear();
    enemies.push_back(EnemyTank(100, 100));
    enemies.push_back(EnemyTank(300, 100));
    for (auto& enemy : enemies) {
        enemy.setImage(enemyTankImage);
    }
}

// Tao mot ke dich moi tai vi tri ngau nhien
void Game::spawnEnemy() {
    int x = rand() % (SCREEN_WIDTH - TILE_SIZE);
    int y = rand() % (SCREEN_HEIGHT / 2);
    enemies.push_back(EnemyTank(x, y));
    enemies.back().setImage(enemyTankImage);
}

// Dat lai tro choi ve trang thai ban dau
void Game::resetGame() {
    player = PlayerTank(SCREEN_WIDTH / 2, SCREEN_HEIGHT - TILE_SIZE);
    player.setImage(playerTankImage);
    player.score = 0; // Reset diem ve 0
    player.hp.resetToFull(); // Hoi day mau truc tiep
    generateWalls();
    generateEnemies();
    healthPacks.clear();
    lastHealthSpawnTime = SDL_GetTicks();
}

// Doc diem cao nhat tu file assets/maX_diem.txt
void Game::loadHighScore() {
    std::ifstream file("assets/maX_diem.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
        std::cout << "Loaded high score: " << highScore << std::endl;
    } else {
        highScore = 0;
        std::cout << "No high score file found in assets/, setting to 0." << std::endl;
    }
}

// Ghi diem cao nhat moi vao file neu diem hien tai cao hon
void Game::saveHighScore(int score) {
    if (score > highScore) {
        highScore = score;
        std::ofstream file("assets/maX_diem.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
            std::cout << "Saved new high score: " << highScore << " to assets/maX_diem.txt" << std::endl;
        } else {
            std::cerr << "Failed to save high score to assets/maX_diem.txt!" << std::endl;
        }
    }
}

// Xu ly cac su kien tu ban phim, chuot, hoac thoat game
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }

        GameState newState = menu->handleEvents(event, gameState);
        if (newState != gameState) {
            if (gameState == GAME_OVER && newState == PLAYING) {
                resetGame();
            }
            gameState = newState;
            continue;
        }

        if (gameState == PLAYING) {
            if (event.type == SDL_MOUSEMOTION) {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                player.move(mouseX, mouseY);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                player.shoot();
                player.bullets.back().setImage(bulletImage);
                amThanh.playSound("ban");
            }
        }
    }
}

/*if (gameState == PLAYING) { // Chi xu ly dieu khien khi dang choi
            if (event.type == SDL_KEYDOWN) { // Kiem tra su kien nhan phim
                switch (event.key.keysym.sym) {
                    case SDLK_UP: // Phim mui ten len: di chuyen xe tang len tren
                        player.move(player.getX(), player.getY() - 10);
                        break;
                    case SDLK_DOWN: // Phim mui ten xuong: di chuyen xe tang xuong duoi
                        player.move(player.getX(), player.getY() + 10);
                        break;
                    case SDLK_LEFT: // Phim mui ten trai: di chuyen xe tang sang trai
                        player.move(player.getX() - 10, player.getY());
                        break;
                    case SDLK_RIGHT: // Phim mui ten phai: di chuyen xe tang sang phai
                        player.move(player.getX() + 10, player.getY());
                        break;
                    case SDLK_0: // Phim 0: ban dan
                        player.shoot();
                        player.bullets.back().setImage(bulletImage);
                        amThanh.playSound("ban");
                        break;
                        */
// Cap nhat trang thai tro choi (vi tri, va cham, sinh ke dich, goi mau)
void Game::update() {
    if (gameState != PLAYING) {
        return;
    }

    background->update();

    player.updateBullets();
    for (auto& enemy : enemies) {
        enemy.update();
        for (auto& bullet : enemy.bullets) {
            bullet.setImage(bulletImage);
        }
    }
    checkCollisions();

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastEnemySpawnTime > ENEMY_SPAWN_INTERVAL) {
        int score = player.score;
        int minEnemies, maxEnemies;

        if (score < 200) {
            minEnemies = 1;
            maxEnemies = 3;
        } else {
            int level = score / 200;
            minEnemies = 1 + (level * 3);
            maxEnemies = 3 + (level * 3);
        }

        int enemyCount = minEnemies + (rand() % (maxEnemies - minEnemies + 1));
        for (int i = 0; i < enemyCount; i++) {
            spawnEnemy();
        }
        lastEnemySpawnTime = currentTime;
    }

    bool hasActiveHealth = false;
    for (const auto& health : healthPacks) {
        if (health.active) {
            hasActiveHealth = true;
            break;
        }
    }
    if (!hasActiveHealth && currentTime - lastHealthSpawnTime > Uint32(10000 + (rand() % 20000))) {
        int x = rand() % (SCREEN_WIDTH - TILE_SIZE / 2);
        int y = rand() % (SCREEN_HEIGHT - TILE_SIZE / 2);
        healthPacks.push_back(HealthPack(x, y));
        healthPacks.back().setImage(healthImage);
        lastHealthSpawnTime = currentTime;
    }
}

// Kiem tra va xu ly va cham giua dan, ke dich, nguoi choi, goi mau
void Game::checkCollisions() {
    for (auto& bullet : player.bullets) {
        for (auto& enemy : enemies) {
            if (SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                bullet.active = false;
                enemy.active = false;
                player.score += 10;
                amThanh.playSound("no");
                std::cout << "Score: " << player.score << std::endl;
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const EnemyTank& e) { return !e.active; }), enemies.end());

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.bullets) {
            if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                bullet.active = false;
                player.hp.dinhchuong(1);
                amThanh.playSound("trung_dan");

                if (player.hp.die()) {
                    saveHighScore(player.score); // Ghi diem cao nhat khi game over
                    gameState = GAME_OVER;
                    std::cout << "Game Over! Final Score: " << player.score << std::endl;
                }
            }
        }
        if (enemy.active && SDL_HasIntersection(&player.rect, &enemy.rect)) {
            enemy.active = false;
            player.hp.dinhchuong(3);
            amThanh.playSound("trung_dan");
            if (player.hp.die()) {
                saveHighScore(player.score); // Ghi diem cao nhat khi game over
                gameState = GAME_OVER;
                std::cout << "Game Over! Final Score: " << player.score << std::endl;
            }
        }
    }

    for (auto& health : healthPacks) {
        if (health.active && SDL_HasIntersection(&player.rect, &health.rect)) {
            health.active = false;
            player.hp.hoiphuc();
            player.hp.hoiphuc();
            player.hp.hoiphuc();
        }
    }
    healthPacks.erase(std::remove_if(healthPacks.begin(), healthPacks.end(),
        [](const HealthPack& h) { return !h.active; }), healthPacks.end());
}

// Ve thanh mau cua nguoi choi
void Game::renderThanhmau() {
    int barWidth = 200;
    int barHeight = 20;
    int barX = (SCREEN_WIDTH - barWidth) / 2;
    int barY = SCREEN_HEIGHT - 30;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect backgroundRect = { barX, barY, barWidth, barHeight };
    SDL_RenderFillRect(renderer, &backgroundRect);

    int currentHealthWidth = (player.hp.getMauHienTai() * barWidth) / player.hp.getMauToiDa();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect healthRect = { barX, barY, currentHealthWidth, barHeight };
    SDL_RenderFillRect(renderer, &healthRect);
}

// Ve cac thanh phan tro choi (nen, tuong, nguoi choi, ke dich, goi mau)
void Game::renderGameElements() {
    background->render(renderer);

    for (auto& wall : walls) {
        wall.render(renderer);
    }

    player.render(renderer);

    for (auto& enemy : enemies) {
        enemy.render(renderer);
    }

    for (auto& health : healthPacks) {
        health.render(renderer);
    }

    renderThanhmau();
}

// Ve toan bo man hinh tro choi (bao gom menu, pause, game over)
void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    renderGameElements();

    switch (gameState) {
        case MENU_SCREEN:
            menu->renderMenuScreen();
            break;
        case PAUSED:
            menu->renderPauseScreen();
            break;
        case GAME_OVER:
            menu->renderGameOverScreen(player.score);
            break;
        case PLAYING:
            break;
    }

    SDL_RenderPresent(renderer);
}

// Chay vong lap chinh cua tro choi
void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        handleEvents();
        update();
        render();

        if (deltaTime < 0.016f) {
            SDL_Delay(16 - (Uint32)(deltaTime * 1000));
        }
    }
}
