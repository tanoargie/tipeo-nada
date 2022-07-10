#include "Game/Game.hpp"
#include "SDL_timer.h"
#include "constants.cpp"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  Game *game = new Game("TipeoNada", SCREEN_WIDTH, SCREEN_HEIGHT);

  while (game->running()) {
    game->handleEvents();
    SDL_AddTimer(3000, &Game::showWord, game);
    game->render();
  }
  return 0;
}
