#include "Game.h"

int speed[3];

int main(int argc, char* argv[]) {
    speed[0] = -4;
    speed[1] = 0;
    speed[2] = 4;
    srand(time(0));
    Game game;
    game.run();
    return 0;
}
