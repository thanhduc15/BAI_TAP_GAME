#pragma once
#include "father_file.h"
#include "Bullet.h"
#include "HP.h"
#include "Image.h"

class PlayerTank {
public:
    SDL_Rect rect;
    std::vector<Bullet> bullets;
    int score;
    HP hp;
    Image* tankImage;

    PlayerTank(int x, int y);
    void move(int x, int y);
    void shoot();
    void updateBullets();
    void render(SDL_Renderer* renderer);
    void setImage(Image* img) { tankImage = img; }
};
