#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Wall.h"
#include "Image.h"
#include "Background.h"
#include "Menu.h"
#include "HealthPack.h"
#include "am_thanh.h" // Giữ nguyên như bạn đã gửi
#include <fstream>    // Đã có để đọc/ghi file

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    PlayerTank player;
    std::vector<EnemyTank> enemies;
    std::vector<Wall> walls;
    std::vector<HealthPack> healthPacks;
    Image* playerTankImage;
    Image* enemyTankImage;
    Image* bulletImage;
    Image* wallImage;
    Image* healthImage;
    Background* background;
    Menu* menu;
    AmThanh& amThanh;
    bool running;
    Uint32 lastEnemySpawnTime;
    Uint32 lastHealthSpawnTime;
    GameState gameState;
    int highScore; // Biến lưu điểm cao nhất

    bool loadResources();
    void generateWalls();
    void generateEnemies();
    void spawnEnemy();
    void resetGame();
    void checkCollisions();
    void renderThanhmau();
    void renderGameElements();
    void loadHighScore(); // Đọc điểm cao nhất từ file
    void saveHighScore(int score); // Ghi điểm cao nhất vào file

public:
    Game();
    ~Game();
    void handleEvents();
    void update();
    void render();
    void run();
};
