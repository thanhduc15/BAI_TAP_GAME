#include "Image.h"
#include <SDL_image.h>

Image::Image(SDL_Renderer* renderer, const std::string& filePath)
    : renderer(renderer), texture(nullptr), width(0), height(0) {
    load(filePath);
}

Image::~Image() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

bool Image::load(const std::string& filePath) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
       // std::cerr << "Failed to load image: " << filePath << " - " << IMG_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
     //   std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return false;
    }

    width = surface->w;
    height = surface->h;
    SDL_FreeSurface(surface);

    return true;
}

void Image::render(int x, int y) {
    SDL_Rect destRect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void Image::render(const SDL_Rect& destRect) {
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}
