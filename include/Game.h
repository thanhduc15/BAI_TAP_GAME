#pragma once

#include "father_file.h" // Đã có các hằng số và GameState
#include "am_thanh.h"
#include "Background.h"
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Wall.h"
#include "Menu.h"
#include "HealthPack.h"

extern int speed[3]; // Giữ lại nếu cần

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    PlayerTank player;
    Image* playerTankImage;
    Image* enemyTankImage;
    Image* bulletImage;
    Image* wallImage;
    Image* healthImage;
    std::vector<Wall> walls;
    std::vector<EnemyTank> enemies;
    std::vector<HealthPack> healthPacks;
    Background* background;
    Menu* menu;
    AmThanh& amThanh;

    bool running;
    Uint32 lastEnemySpawnTime;
    Uint32 lastHealthSpawnTime;
    GameState gameState;

    bool loadResources();
    void generateWalls();
    void generateEnemies();
    void spawnEnemy();
    void resetGame();

    void handleEvents();
    void update();
    void checkCollisions();
    void render();
    void renderGameElements();
    void renderThanhmau();
};
