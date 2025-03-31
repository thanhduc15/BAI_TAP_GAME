#pragma once
#include "father_file.h"
#include "Image.h"

class HealthPack {
public:
    SDL_Rect rect;
    bool active;
    Image* healthImage;

    HealthPack(int x, int y);
    void render(SDL_Renderer* renderer);
    void setImage(Image* img) { healthImage = img; }
};
