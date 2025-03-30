#pragma once
#include "father_file.h"
#include "Image.h"

class Wall {
public:
    SDL_Rect rect;
    Image* wallImage;

    Wall(int x, int y);
    void render(SDL_Renderer* renderer);
    void setImage(Image* img) { wallImage = img; }
};
