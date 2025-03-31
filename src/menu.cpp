#include "Menu.h"
#include <iostream>

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), font(nullptr) {
    textColor = {255, 255, 0, 255}; // Yellow text
}

Menu::~Menu() {
    if (font) {
        TTF_CloseFont(font);
    }
}

bool Menu::loadFont() {
    font = TTF_OpenFont("assets/arial.ttf", 28);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void Menu::renderText(const std::string& text, int x, int y) {
    if (!font) return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Menu::renderMenuScreen() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    renderText("BATTLE CITY GALAXY", SCREEN_WIDTH/2 - 150, 150);
    renderText("PRESS SPACE TO START", SCREEN_WIDTH/2 - 150, 300);
    renderText("USE MOUSE TO MOVE AND SHOOT", SCREEN_WIDTH/2 - 250, 350);
}

void Menu::renderPauseScreen() {
    renderText("PAUSED", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2 - 50);
    renderText("PRESS SPACE TO CONTINUE", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2);
    renderText("PRESS ESC TO QUIT", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 + 50);
}

void Menu::renderGameOverScreen(int score) {
    std::string scoreText = "YOUR SCORE: " + std::to_string(score);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 150, 0, 0, 180);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    renderText("GAME OVER", SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 - 100);
    renderText(scoreText.c_str(), SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2);
    renderText("PRESS ENTER TO PLAY AGAIN", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 50);
}

GameState Menu::handleEvents(SDL_Event& event, GameState currentState) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (currentState == MENU_SCREEN || currentState == GAME_OVER) return PLAYING;
                if (currentState == PAUSED) return PLAYING;
                break;

            case SDLK_ESCAPE:
                if (currentState == PLAYING) return PAUSED;
                if (currentState == PAUSED) return MENU_SCREEN;
                break;

            case SDLK_RETURN:
                if (currentState == GAME_OVER) return PLAYING;
                break;
        }
    }
    return currentState;
}
