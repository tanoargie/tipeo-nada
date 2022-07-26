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

  difficultyEnum diffEasy = EASY;
  difficultyEnum diffMedium = MEDIUM;
  difficultyEnum diffHard = HARD;

  function<void()> diffEasyFn = [&game, diffEasy]() {
    game->difficulty = diffEasy;
  };

  function<void()> diffMediumFn = [&game, diffMedium]() {
    game->difficulty = diffMedium;
  };

  function<void()> diffHardFn = [&game, diffHard]() {
    game->difficulty = diffHard;
  };

  SDL_Rect dstEasy, dstMedium, dstHard;

  int easyYPos = SCREEN_HEIGHT / 4;

  dstEasy.x = SCREEN_WIDTH / 2;
  dstEasy.y = easyYPos;
  TTF_SizeUTF8(game->font, getDifficultyChar(diffEasy), &dstEasy.w, &dstEasy.h);
  dstMedium.x = SCREEN_WIDTH / 2;
  dstMedium.y = easyYPos + 50;
  TTF_SizeUTF8(game->font, getDifficultyChar(diffMedium), &dstMedium.w,
               &dstMedium.h);
  dstHard.x = SCREEN_WIDTH / 2;
  dstHard.y = easyYPos + 100;
  TTF_SizeUTF8(game->font, getDifficultyChar(diffHard), &dstHard.w, &dstHard.h);

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
        if (game->difficulty == EASY) {
          Uint32 timerIdShowWord = SDL_AddTimer(3000, &Game::showWord, game);
        } else if (game->difficulty == MEDIUM) {
          Uint32 timerIdShowWord = SDL_AddTimer(1500, &Game::showWord, game);
        } else if (game->difficulty == HARD) {
          Uint32 timerIdShowWord = SDL_AddTimer(500, &Game::showWord, game);
        }
        Uint32 timerIdUpdateWordsLocation =
            SDL_AddTimer(250, &Game::updateWordsLocation, game);
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
