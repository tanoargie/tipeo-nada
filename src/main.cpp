#include "Game/Game.hpp"

#include <stdio.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
void one_iter(void *userData) {
  Game *game = static_cast<Game *>(userData);

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

  const char *diffMessage = "Choose difficulty, please:";

  SDL_Rect dstEasy, dstMedium, dstHard, dstMessage;
  TTF_SizeUTF8(game->font, diffMessage, &dstMessage.w, &dstMessage.h);
  dstMessage.x = SCREEN_WIDTH / 2 - (dstMessage.w / 2);
  dstMessage.y = SCREEN_HEIGHT / 2 - (dstMessage.h / 2) - 100;

  TTF_SizeUTF8(game->font, getDifficultyChar(diffEasy), &dstEasy.w, &dstEasy.h);
  dstEasy.x = SCREEN_WIDTH / 2 - (dstEasy.w / 2);
  dstEasy.y = dstMessage.y + 100;

  TTF_SizeUTF8(game->font, getDifficultyChar(diffMedium), &dstMedium.w,
               &dstMedium.h);
  dstMedium.x = SCREEN_WIDTH / 2 - (dstMedium.w / 2);
  dstMedium.y = dstEasy.y + 50;

  TTF_SizeUTF8(game->font, getDifficultyChar(diffHard), &dstHard.w, &dstHard.h);
  dstHard.x = SCREEN_WIDTH / 2 - (dstHard.w / 2);
  dstHard.y = dstMedium.y + 50;

  game->handleEvents();

  if (!game->sessionEnded && game->difficulty == NOT_SET) {
    game->renderClear();
    game->addText(diffMessage, &dstMessage);
    game->addButton(getDifficultyChar(diffEasy), &diffEasyFn, &dstEasy);
    game->addButton(getDifficultyChar(diffMedium), &diffMediumFn, &dstMedium);
    game->addButton(getDifficultyChar(diffHard), &diffHardFn, &dstHard);
    game->render();
  } else if (!game->sessionEnded) {
    if (game->timerIdShowWord == 0 || game->timerIdUpdateWordsLocation == 0) {
      if (game->difficulty == EASY) {
        game->timerIdShowWord =
            emscripten_set_interval(&Game::showWord, 3000, userData);
      } else if (game->difficulty == MEDIUM) {
        game->timerIdShowWord =
            emscripten_set_interval(&Game::showWord, 1500, userData);
      } else if (game->difficulty == HARD) {
        game->timerIdShowWord =
            emscripten_set_interval(&Game::showWord, 500, userData);
      }
      game->timerIdUpdateWordsLocation =
          emscripten_set_interval(&Game::updateWordsLocation, 250, userData);
    }
    game->sessionEnded = true;
  }
}
#endif

int main(int argc, char *argv[]) {
  Game *game = new Game();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(one_iter, game, 0, 1);
#else
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

  const char *diffMessage = "Choose difficulty, please:";

  SDL_Rect dstEasy, dstMedium, dstHard, dstMessage;
  TTF_SizeUTF8(game->font, diffMessage, &dstMessage.w, &dstMessage.h);
  dstMessage.x = SCREEN_WIDTH / 2 - (dstMessage.w / 2);
  dstMessage.y = SCREEN_HEIGHT / 2 - (dstMessage.h / 2) - 100;

  TTF_SizeUTF8(game->font, getDifficultyChar(diffEasy), &dstEasy.w, &dstEasy.h);
  dstEasy.x = SCREEN_WIDTH / 2 - (dstEasy.w / 2);
  dstEasy.y = dstMessage.y + 100;

  TTF_SizeUTF8(game->font, getDifficultyChar(diffMedium), &dstMedium.w,
               &dstMedium.h);
  dstMedium.x = SCREEN_WIDTH / 2 - (dstMedium.w / 2);
  dstMedium.y = dstEasy.y + 50;

  TTF_SizeUTF8(game->font, getDifficultyChar(diffHard), &dstHard.w, &dstHard.h);
  dstHard.x = SCREEN_WIDTH / 2 - (dstHard.w / 2);
  dstHard.y = dstMedium.y + 50;

  while (game->running()) {
    game->handleEvents();

    if (!game->sessionEnded && game->difficulty == NOT_SET) {
      game->renderClear();
      game->addText(diffMessage, &dstMessage);
      game->addButton(getDifficultyChar(diffEasy), &diffEasyFn, &dstEasy);
      game->addButton(getDifficultyChar(diffMedium), &diffMediumFn, &dstMedium);
      game->addButton(getDifficultyChar(diffHard), &diffHardFn, &dstHard);
      game->render();
    } else if (!game->sessionEnded) {
      if (game->timerIdShowWord == 0 || game->timerIdUpdateWordsLocation == 0) {
        if (game->difficulty == EASY) {
          game->timerIdShowWord = SDL_AddTimer(3000, &Game::showWord, game);
        } else if (game->difficulty == MEDIUM) {
          game->timerIdShowWord = SDL_AddTimer(1500, &Game::showWord, game);
        } else if (game->difficulty == HARD) {
          game->timerIdShowWord = SDL_AddTimer(500, &Game::showWord, game);
        }
        game->timerIdUpdateWordsLocation =
            SDL_AddTimer(250, &Game::updateWordsLocation, game);
      }
      game->sessionEnded = true;
    }
  }
#endif

  return 0;
}
