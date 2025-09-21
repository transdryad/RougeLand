#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "game.hpp"

int main(int argc, char* argv[]) {
    Game game(argc, argv);
    //before this
    game.run();
}
