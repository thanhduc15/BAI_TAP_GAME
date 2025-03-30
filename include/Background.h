#pragma once
#include "father_file.h"
#include "Image.h"

class Background {
private:
    Image* backgroundImage;
    int bgY1, bgY2; // Thay đổi từ bgX1, bgX2 thành bgY1, bgY2
    int scrollSpeed;

    struct Star {
        int x, y;
        int speed;
        Uint8 alpha;
    };
    std::vector<Star> stars;

    void initStars(int count);

public:
    Background(SDL_Renderer* renderer, const std::string& imagePath);
    ~Background();

    void update();
    void render(SDL_Renderer* renderer);

    void setScrollSpeed(int speed) { scrollSpeed = speed; }
};
