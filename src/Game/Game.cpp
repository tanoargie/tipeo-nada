#include "Game.hpp"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_ttf.h"

Game::Game(const char *title) {
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && IMG_Init(IMG_INIT_PNG) &&
      TTF_Init() == 0) {
    cout << "Initialized!" << endl;
    window = SDL_CreateWindow("TipeoNada", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    if (!window) {
      printf("Could not create window: %s\n", SDL_GetError());
      isRunning = false;
      return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    wordsOnScreen = new map<string, pair<int, int>>();

    if (!renderer) {
      printf("Could not create renderer: %s\n", SDL_GetError());
      isRunning = false;
      return;
    }

    font = TTF_OpenFont("OpenSans-Bold.ttf", 24);
    if (font == NULL) {
      cout << SDL_GetError();
    }

    fontColor.r = 255;
    fontColor.g = 255;
    fontColor.b = 255;
    fontColor.a = 0;

    srand(time(0));

    ifstream wordfile("words.txt");
    string line;

    while (getline(wordfile, line)) {
      words.push_back(line);
    }
    wordfile.close();

    isRunning = true;
  } else {
    isRunning = false;
  }
}

Game::~Game() {
  wordsOnScreen = NULL;
  delete wordsOnScreen;
  SDL_FreeSurface(backgroundImage);
  SDL_DestroyTexture(backgroundTex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  IMG_Quit();
  SDL_Quit();
}

Uint32 Game::updateWordsLocation(Uint32 interval, void *param) {
  Game *game = reinterpret_cast<Game *>(param);
  map<string, pair<int, int>>::iterator it;
  SDL_RenderClear(game->renderer);
  for (it = game->wordsOnScreen->begin(); it != game->wordsOnScreen->end();
       it++) {

    SDL_Surface *surfaceMessage =
        TTF_RenderUTF8_Blended(game->font, it->first.c_str(), game->fontColor);

    SDL_Texture *message =
        SDL_CreateTextureFromSurface(game->renderer, surfaceMessage);

    SDL_Rect dst;

    dst.x = it->second.first;
    dst.y = it->second.second;

    pair<int, int> newPosition =
        make_pair(it->second.first, it->second.second + 30);

    game->wordsOnScreen->operator[](it->first) = newPosition;

    TTF_SizeUTF8(game->font, it->first.c_str(), &dst.w, &dst.h);

    SDL_RenderCopy(game->renderer, message, NULL, &dst);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
  }
  SDL_RenderPresent(game->renderer);

  return interval;
}

bool Game::canAddWord() {
  if (difficulty == EASY && wordsOnScreen->size() < 5) {
    return true;
  } else if (difficulty == MEDIUM && wordsOnScreen->size() < 10) {
    return true;
  } else if (difficulty == HARD && wordsOnScreen->size() < 20) {
    return true;
  }
  return false;
}

Uint32 Game::showWord(Uint32 interval, void *param) {
  Game *game = reinterpret_cast<Game *>(param);
  int random = rand() % game->words.size();
  int randomX = rand() % (SCREEN_WIDTH - 100);

  string randomWord = game->words.at(random);
  if (randomX > 1100 && randomWord.size() > 5) {
    randomX -= 300;
  }

  pair<int, int> position = make_pair(randomX, 0);

  if (game->canAddWord()) {
    game->wordsOnScreen->insert(
        pair<string, pair<int, int>>(randomWord, position));
  }

  return interval;
}

void Game::removeWord() { wordsOnScreen->erase(wordTyping); }

bool Game::isWordTypingOnScreen() {
  if (wordsOnScreen->find(wordTyping) != wordsOnScreen->end()) {
    return true;
  }
  return false;
}

void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    isRunning = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_RETURN) {
      if (isWordTypingOnScreen()) {
        removeWord();
      }
      wordTyping.clear();
    } else {
      const char *newChar = SDL_GetKeyName(event.key.keysym.sym);
      wordTyping += tolower(*newChar);
    }
    break;
  default:
    break;
  }
}

void Game::render() { SDL_RenderPresent(renderer); }

bool Game::running() { return isRunning; }
