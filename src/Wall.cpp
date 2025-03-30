#include "Wall.h"

Wall::Wall(int x, int y) : wallImage(nullptr) {
    rect.x = x;
    rect.y = y;
    rect.w = TILE_SIZE;
    rect.h = TILE_SIZE;
}

void Wall::render(SDL_Renderer* renderer) {
    if (wallImage) {
        wallImage->render(rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
