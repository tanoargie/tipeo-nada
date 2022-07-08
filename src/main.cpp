#include "Game/Game.hpp"

#include <stdio.h>
#include <stdlib.h>

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main() {
  Game *game = new Game("TipeoNada", SCREEN_WIDTH, SCREEN_HEIGHT);

  while (game->running()) {
    game->handleEvents();
    game->update();
    game->render();
  }
  return 0;
}
