#include "Menu.h"

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), font(nullptr) {
    textColor = {255, 255, 255, 255};
}

Menu::~Menu() {
    if (font) {
        TTF_CloseFont(font);
    }
}

void Menu::loadFont() {
    // Kiểm tra xem SDL_ttf đã khởi tạo chưa
    if (!TTF_WasInit()) {
        std::cerr << "SDL_ttf is not initialized! Please check TTF_Init() in Game constructor." << std::endl;
        return;
    }

    // Load font từ đường dẫn Windows
    font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 28);
    if (!font) {
        std::cerr << "Failed to load font from C:/Windows/Fonts/arial.ttf: " << TTF_GetError() << std::endl;
        std::cerr << "Suggestions:" << std::endl;
        std::cerr << "1. Ensure SDL2_ttf.dll is in the project directory." << std::endl;
        std::cerr << "2. Add -lSDL2_ttf to linker settings in your IDE." << std::endl;
        std::cerr << "3. Verify Arial font exists at C:/Windows/Fonts/arial.ttf." << std::endl;
    } else {
        std::cout << "Successfully loaded font from C:/Windows/Fonts/arial.ttf!" << std::endl;
    }
}

void Menu::renderText(const std::string& text, int x, int y) {
    if (!font) {
        // Chỉ in lỗi một lần để tránh spam console
        static bool errorPrinted = false;
        if (!errorPrinted) {
            std::cerr << "Font not loaded, cannot render text!" << std::endl;
            errorPrinted = true;
        }
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!surface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Menu::renderMenuScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    renderText("BATTLE CITY GALAXY", 250, 150);
    renderText("Press SPACE to Start", 270, 300);
    renderText("Use Mouse to Move and Shoot", 220, 350);
}

void Menu::renderPauseScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    renderText("PAUSED", 350, 250);
    renderText("Press SPACE to Continue", 250, 300);
    renderText("Press ESC to Return to Menu", 230, 350);
}

void Menu::renderGameOverScreen(int score) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    renderText("GAME OVER", 320, 200);
    renderText("Your Score: " + std::to_string(score), 320, 250);
    renderText("Press ENTER to Play Again", 250, 350);
    renderText("Press ESC to Quit", 290, 400);
}

GameState Menu::handleEvents(SDL_Event& event, GameState currentState) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (currentState == MENU_SCREEN) {
                    return PLAYING;
                }
                if (currentState == PAUSED) {
                    return PLAYING;
                }
                break;

            case SDLK_ESCAPE:
                if (currentState == PLAYING) {
                    return PAUSED;
                }
                if (currentState == PAUSED) {
                    return MENU_SCREEN;
                }
                if (currentState == GAME_OVER) {
                    SDL_Quit();
                    exit(0);
                }
                break;

            case SDLK_RETURN:
                if (currentState == GAME_OVER) {
                    return PLAYING;
                }
                break;
        }
    }
    return currentState;
}
