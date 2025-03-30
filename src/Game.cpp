#include "Game.h"

Game::Game() :
    player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - TILE_SIZE),
    playerTankImage(nullptr),
    enemyTankImage(nullptr),
    bulletImage(nullptr),
    wallImage(nullptr),
    background(nullptr),
    menu(nullptr),
    amThanh(AmThanh::getInstance()),
    running(true),
    lastEnemySpawnTime(0),
    gameState(MENU_SCREEN) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
       // std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    if (!amThanh.init()) {
      //  std::cerr << "Failed to initialize sound system!" << std::endl;
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
}

Game::~Game() {
    delete playerTankImage;
    delete enemyTankImage;
    delete bulletImage;
    delete wallImage;
    delete background;
    delete menu;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::loadResources() {
    playerTankImage = new Image(renderer, "assets/player_tank.png");
    enemyTankImage = new Image(renderer, "assets/enemy_tank.png");
    bulletImage = new Image(renderer, "assets/bullet.png");
    wallImage = new Image(renderer, "assets/wall.png");

    if (!playerTankImage->getTexture() || !enemyTankImage->getTexture() ||
        !bulletImage->getTexture() || !wallImage->getTexture()) {
        std::cerr << "Failed to load one or more images!" << std::endl;
        return false;
    }

    return true;
}

void Game::generateWalls() {
    walls.clear();
    for (int i = 0; i < SCREEN_WIDTH; i += TILE_SIZE) {
        walls.push_back(Wall(i, SCREEN_HEIGHT - TILE_SIZE));
        walls.back().setImage(wallImage);
    }
}

void Game::generateEnemies() {
    enemies.clear();
    enemies.push_back(EnemyTank(100, 100));
    enemies.push_back(EnemyTank(300, 100));
    for (auto& enemy : enemies) {
        enemy.setImage(enemyTankImage);
    }
}

void Game::spawnEnemy() {
    int x = rand() % (SCREEN_WIDTH - TILE_SIZE);
    int y = rand() % (SCREEN_HEIGHT / 2);
    enemies.push_back(EnemyTank(x, y));
    enemies.back().setImage(enemyTankImage);
}

void Game::resetGame() {
    player = PlayerTank(SCREEN_WIDTH / 2, SCREEN_HEIGHT - TILE_SIZE);
    player.setImage(playerTankImage);
    player.score = 0;
    generateWalls();
    generateEnemies();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }

        GameState newState = menu->handleEvents(event, gameState);
        if (newState != gameState) {
            gameState = newState;

            if (gameState == PLAYING && newState != PAUSED) {
                resetGame();
            }
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
        int enemyCount = std::max(1, rand() % 4);
        for (int i = 0; i < enemyCount; i++) {
            spawnEnemy();
        }
        lastEnemySpawnTime = currentTime;
    }
}

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
                    gameState = GAME_OVER;
                    std::cout << "Game Over! Final Score: " << player.score << std::endl;
                }
            }
        }
    }
}

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

void Game::renderGameElements() {
    background->render(renderer);

    for (auto& wall : walls) {
        wall.render(renderer);
    }

    player.render(renderer);

    for (auto& enemy : enemies) {
        enemy.render(renderer);
    }

    renderThanhmau();
}

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
