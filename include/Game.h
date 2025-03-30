#ifndef GAME_H
#define GAME_H

#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Wall.h"
#include "Background.h"
#include "am_thanh.h"
#include "Menu.h"
#include "father_file.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    bool loadResources();
    void generateWalls();
    void generateEnemies();
    void spawnEnemy();
    void resetGame();

    void handleEvents();
    void update();
    void render();
    void renderGameElements();
    void checkCollisions();
    void renderThanhmau();

    SDL_Window* window;
    SDL_Renderer* renderer;

    PlayerTank player;
    std::vector<EnemyTank> enemies;
    std::vector<Wall> walls;

    Image* playerTankImage;
    Image* enemyTankImage;
    Image* bulletImage;
    Image* wallImage;
    Background* background;
    Menu* menu;

    AmThanh& amThanh;

    bool running;
    Uint32 lastEnemySpawnTime;
    GameState gameState;
};

#endif
