#include "HealthPack.h"

HealthPack::HealthPack(int x, int y) : active(true), healthImage(nullptr) {
    rect.x = x;
    rect.y = y;
    rect.w = TILE_SIZE / 2; // Kích thước nhỏ hơn xe tăng
    rect.h = TILE_SIZE / 2;
}

void HealthPack::render(SDL_Renderer* renderer) {
    if (healthImage) {
        healthImage->render(rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Màu xanh lá nếu không có hình
        SDL_RenderFillRect(renderer, &rect);
    }
}
