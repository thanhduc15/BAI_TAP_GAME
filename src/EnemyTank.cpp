#include "EnemyTank.h"
#include "am_thanh.h"
#include <cstdlib>

EnemyTank::EnemyTank(int x, int y) : dx(1), dy(1), active(true),lastShootTime(0), tankImage(nullptr) {
    rect.x = x;
    rect.y = 0;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
}

void EnemyTank::update() {
    int itx = rand() % 3;
    int ity = rand() % 3;
    rect.x += speed[itx];
    rect.y += abs(speed[ity]) / 2;
    if (rect.x < 0 || rect.x + rect.w > SCREEN_WIDTH) dx = -dx;
    if (rect.y < 0 || rect.y + rect.h > SCREEN_HEIGHT) dy = -dy;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastShootTime > ENEMY_SHOOT_INTERVAL) {
        shoot();
        lastShootTime = currentTime;
    }

    for (auto& bullet : bullets) {
        bullet.update();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) {
        return !b.active;
    }), bullets.end());
}

void EnemyTank::shoot() {
    bullets.push_back(Bullet(rect.x + rect.w / 2, rect.y + rect.h, 5));
    AmThanh::getInstance().playSound("ban");
}

void EnemyTank::render(SDL_Renderer* renderer) {
    if (tankImage) {
        tankImage->render(rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }
}
