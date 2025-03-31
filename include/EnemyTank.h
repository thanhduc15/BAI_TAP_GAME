#pragma once
#include "father_file.h"
#include "Bullet.h"
#include "Image.h"

class EnemyTank {
public:
    SDL_Rect rect;
    bool active; // Đặt trước dx, dy để khớp với thứ tự khởi tạo
    std::vector<Bullet> bullets;
    Uint32 lastShootTime;
    int dx, dy;
    Image* tankImage;

    EnemyTank(int x, int y);
    void update();
    void shoot();
    void render(SDL_Renderer* renderer);
    void setImage(Image* img) { tankImage = img; }
};
