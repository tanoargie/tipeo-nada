#include "Game/Game.hpp"
#include "SDL.h"
#include "SDL_error.h"
#include "constants.cpp"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  Game *game = new Game("TipeoNada", SCREEN_WIDTH, SCREEN_HEIGHT);

  if (SDL_Init(SDL_INIT_TIMER) < 0) {
    cout << SDL_GetError();
  }
  Uint32 timerIdShowWord = SDL_AddTimer(3000, &Game::showWord, game);
  Uint32 timerIdUpdateWordsLocation =
      SDL_AddTimer(1000, &Game::updateWordsLocation, game);

  while (game->running()) {
    game->handleEvents();
  }

  SDL_RemoveTimer(timerIdShowWord);
  SDL_RemoveTimer(timerIdUpdateWordsLocation);
  return 0;
}
