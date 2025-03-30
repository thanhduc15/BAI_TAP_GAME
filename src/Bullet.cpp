#include "Bullet.h"

Bullet::Bullet(int x, int y, int dy) : dy(dy), active(true), bulletImage(nullptr) {
    rect.x = x;
    rect.y = y;
    rect.w = 10;
    rect.h = 10;
}

void Bullet::update() {
    rect.y += dy;
    if (rect.y < 0 || rect.y > SCREEN_HEIGHT) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (bulletImage) {
        bulletImage->render(rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
