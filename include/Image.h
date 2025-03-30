#pragma once
#include "father_file.h"

class Image {
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    int width;
    int height;

public:
    Image(SDL_Renderer* renderer, const std::string& filePath);
    ~Image();

    bool load(const std::string& filePath);
    void render(int x, int y);
    void render(const SDL_Rect& destRect);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    SDL_Texture* getTexture() const { return texture; }
};
