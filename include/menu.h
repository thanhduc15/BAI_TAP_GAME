#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "father_file.h"

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void loadFont();
    void renderMenuScreen();
    void renderPauseScreen();
    void renderGameOverScreen(int score);
    GameState handleEvents(SDL_Event& event, GameState currentState);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color textColor;

    void renderText(const std::string& text, int x, int y);
};

#endif
