#include "Game/Game.hpp"

#include <SDL_error.h>
#include <SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  Game *game = new Game("TipeoNada");

  if (SDL_Init(SDL_INIT_TIMER) < 0) {
    cout << SDL_GetError();
  }

  function<void(difficultyEnum)> changeGameDiff = [&game](difficultyEnum diff) {
    game->changeDifficulty(diff);
    game->renderClear();
    game->render();
  };

  difficultyEnum diffEasy = EASY;

  function<void()> diffEasyFn = [diffEasy, &changeGameDiff]() {
    changeGameDiff(diffEasy);
  };

  const char *easyMessage = "Easy";

  SDL_Rect dstEasy;

  int easyYPos = SCREEN_HEIGHT / 4;

  dstEasy.x = SCREEN_WIDTH / 2;
  dstEasy.y = easyYPos;

  game->addButton(easyMessage, &diffEasyFn, &dstEasy);

  Uint32 timerIdShowWord;
  Uint32 timerIdUpdateWordsLocation;

  bool setTimer = false;
  while (game->running()) {
    game->handleEvents();

    if (game->getDifficulty() != NOT_SET) {
      // ya eligió dificultad

      if (!setTimer) {
        Uint32 timerIdShowWord = SDL_AddTimer(3000, &Game::showWord, game);
        Uint32 timerIdUpdateWordsLocation =
            SDL_AddTimer(1000, &Game::updateWordsLocation, game);
        setTimer = true;
      }
    }
  }

  if (setTimer) {
    SDL_RemoveTimer(timerIdShowWord);
    SDL_RemoveTimer(timerIdUpdateWordsLocation);
  }

  return 0;
}
