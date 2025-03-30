#include "Background.h"

Background::Background(SDL_Renderer* renderer, const std::string& imagePath)
    : scrollSpeed(1) {
    backgroundImage = new Image(renderer, imagePath);
    bgY1 = 0;
    bgY2 = -backgroundImage->getHeight(); // Đặt ảnh thứ 2 phía trên ảnh đầu tiên
    initStars(100);
}

Background::~Background() {
    delete backgroundImage;
}

void Background::update() {
    bgY1 += scrollSpeed; // Di chuyển xuống dưới
    bgY2 += scrollSpeed;

    // Reset position khi ảnh chạy hết
    if (bgY1 >= backgroundImage->getHeight()) {
        bgY1 = -backgroundImage->getHeight();
    }
    if (bgY2 >= backgroundImage->getHeight()) {
        bgY2 = -backgroundImage->getHeight();
    }

    // Cập nhật sao băng (di chuyển xuống dưới)
    for (auto& star : stars) {
        star.y += star.speed;
        if (star.y > SCREEN_HEIGHT) {
            star.y = 0;
            star.x = rand() % SCREEN_WIDTH;
        }
    }
}

void Background::render(SDL_Renderer* renderer) {
    // Vẽ 2 ảnh nối tiếp nhau theo chiều dọc
    SDL_Rect dest1 = {0, bgY1, SCREEN_WIDTH, backgroundImage->getHeight()};
    SDL_Rect dest2 = {0, bgY2, SCREEN_WIDTH, backgroundImage->getHeight()};

    SDL_RenderCopy(renderer, backgroundImage->getTexture(), nullptr, &dest1);
    SDL_RenderCopy(renderer, backgroundImage->getTexture(), nullptr, &dest2);

    // Vẽ sao băng
    for (const auto& star : stars) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, star.alpha);
        SDL_RenderDrawPoint(renderer, star.x, star.y);
    }
}

void Background::initStars(int count) {
    for (int i = 0; i < count; i++) {
        stars.push_back({
            rand() % SCREEN_WIDTH,
            rand() % SCREEN_HEIGHT,
            1 + rand() % 5,
            (Uint8)(100 + rand() % 155)
        });
    }
}

