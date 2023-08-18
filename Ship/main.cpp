#include <iostream>
#include<fmt/core.h>
#include<fmt/color.h>
#include "Game.h"
#undef main // SDL2 defines a main macro, so we need to undefine it before we define our own main function

int main(int argc,char* argv[]) {
    Game game = Game();
    bool success = game.Initialize();
    if (success){
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}