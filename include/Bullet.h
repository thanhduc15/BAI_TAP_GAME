#pragma once
#include "father_file.h"
#include "Image.h"

class Bullet {
public:
    SDL_Rect rect;
    int dy;
    bool active;
    Image* bulletImage;

    Bullet(int x, int y, int dy);
    void update();
    void render(SDL_Renderer* renderer);
    void setImage(Image* img) { bulletImage = img; }
};
