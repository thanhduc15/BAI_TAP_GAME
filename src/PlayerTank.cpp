#include "PlayerTank.h"
#include "am_thanh.h"

PlayerTank::PlayerTank(int x, int y) : score(0), tankImage(nullptr) {
    rect.x = x;
    rect.y = y;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
}

void PlayerTank::move(int x, int y) {
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;
    if (rect.x + rect.w > SCREEN_WIDTH) rect.x = SCREEN_WIDTH - rect.w;
    if (rect.y + rect.h > SCREEN_HEIGHT) rect.y = SCREEN_HEIGHT - rect.h;
}

void PlayerTank::shoot() {
    bullets.push_back(Bullet(rect.x + rect.w / 2, rect.y, -5));
    AmThanh::getInstance().playSound("ban");
}

void PlayerTank::updateBullets() {
    for (auto& bullet : bullets) {
        bullet.update();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) {
        return !b.active;
    }), bullets.end());
}

void PlayerTank::render(SDL_Renderer* renderer) {
    if (tankImage) {
        tankImage->render(rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }
}
