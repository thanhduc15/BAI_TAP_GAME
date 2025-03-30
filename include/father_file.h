#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 60;
const int ENEMY_SHOOT_INTERVAL = 2000;
const int ENEMY_SPAWN_INTERVAL = 2000;

extern int speed[3];

enum GameState {
    MENU_SCREEN,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Image;
class Wall;
class Bullet;
class HP;
class PlayerTank;
class EnemyTank;
class Game;
class Menu;
