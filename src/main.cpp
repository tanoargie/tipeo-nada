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
    game->difficulty = diff;
  };

  difficultyEnum diffEasy = EASY;
  difficultyEnum diffMedium = MEDIUM;
  difficultyEnum diffHard = HARD;

  function<void()> diffEasyFn = [diffEasy, &changeGameDiff]() {
    changeGameDiff(diffEasy);
  };
  function<void()> diffMediumFn = [diffMedium, &changeGameDiff]() {
    changeGameDiff(diffMedium);
  };
  function<void()> diffHardFn = [diffHard, &changeGameDiff]() {
    changeGameDiff(diffHard);
  };

  const char *easyMessage = "Easy";
  const char *mediumMessage = "Medium";
  const char *hardMessage = "Hard";

  SDL_Rect dstEasy, dstMedium, dstHard;

  int easyYPos = SCREEN_HEIGHT / 4;

  dstEasy.x = SCREEN_WIDTH / 2;
  dstEasy.y = easyYPos;
  dstEasy.h = 100;
  dstEasy.w = 100;
  dstMedium.x = SCREEN_WIDTH / 2;
  dstMedium.y = easyYPos + 50;
  dstMedium.h = 100;
  dstMedium.w = 100;
  dstHard.x = SCREEN_WIDTH / 2;
  dstHard.y = easyYPos + 100;
  dstHard.w = 100;
  dstHard.h = 100;

  game->renderClear();
  game->addButton(getDifficultyChar(diffEasy), &diffEasyFn, &dstEasy);
  game->addButton(getDifficultyChar(diffMedium), &diffMediumFn, &dstMedium);
  game->addButton(getDifficultyChar(diffHard), &diffHardFn, &dstHard);
  game->render();

  Uint32 timerIdShowWord;
  Uint32 timerIdUpdateWordsLocation;

  bool setTimer = false;
  while (game->running()) {
    game->handleEvents();

    if (game->difficulty != NOT_SET) {
      // ya eligió dificultad

      if (!setTimer) {
        Uint32 timerIdShowWord = SDL_AddTimer(2000, &Game::showWord, game);
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
