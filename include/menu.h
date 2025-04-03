#pragma once
#include "father_file.h"
#include <SDL_ttf.h>

class Menu {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color textColor;
    SDL_Texture* menuTexture;

public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void loadFont();
    void loadMenuImage();
    void renderText(const std::string& text, int x, int y);
    void renderMenuScreen();
    void renderPauseScreen();
    void renderGameOverScreen(int score);
    GameState handleEvents(SDL_Event& event, GameState currentState);
};
