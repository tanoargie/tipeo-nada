#include "Game/Game.hpp"

#include <stdio.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
void one_iter(void *userData) {
  Game *game = static_cast<Game *>(userData);
  game->gameLoop();
  game->handleEvents();
}
#endif

int main(int argc, char *argv[]) {
  Game *game = new Game();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(one_iter, game, 0, 1);
#else
  while (game->running()) {
    game->gameLoop();
    game->handleEvents();
  }
  
#endif

  return 0;
}
