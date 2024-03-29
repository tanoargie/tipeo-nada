#include "Game.hpp"

bool Game::initializeAudio() {
#ifdef __EMSCRIPTEN__
  if (Mix_Init(MIX_INIT_MP3) == 0) {
    return true;
  }
  printf("Init emscripten audio failed: %s\n", SDL_GetError());
  return false;
#else
  if (Mix_Init(MIX_INIT_MP3) != 0) {
    return true;
  }
  printf("Init audio failed: %s\n", SDL_GetError());
  return false;
#endif
}

Game::Game(const char *title) {
  if (SDL_Init(SDL_INIT_TIMER) == 0 && IMG_Init(IMG_INIT_PNG) &&
      TTF_Init() == 0 && initializeAudio()) {
    cout << "Initialized!" << endl;

#ifdef __EMSCRIPTEN__
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
#endif
    window = SDL_CreateWindow("TipeoNada", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    if (!window) {
      printf("Could not create window: %s\n", SDL_GetError());
      isRunning = false;
      return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    wordsOnScreen = new map<string, pair<int, int>>();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
      printf("Error opening music: %s\n", Mix_GetError());
    } else {
      backgroundMusic = Mix_LoadMUS("Assets/cyberpunk.mp3");
      Mix_VolumeMusic((20 * MIX_MAX_VOLUME) / 100);

      if (!backgroundMusic) {
        printf("Error loading music: %s\n", Mix_GetError());
      }

      if (Mix_PlayMusic(backgroundMusic, -1) < 0) {
        printf("Error playing music: %s\n", Mix_GetError());
      }
    }

    player = new Player();

    if (!renderer) {
      printf("Could not create renderer: %s\n", SDL_GetError());
      isRunning = false;
      return;
    }

    font = TTF_OpenFont("Assets/OpenSans-Bold.ttf", 24);
    if (font == NULL) {
      cout << SDL_GetError();
    }

    fontColor.r = 255;
    fontColor.g = 255;
    fontColor.b = 255;

    srand(time(0));

    ifstream wordfile("Assets/words.txt");
    string line;

    while (getline(wordfile, line)) {
      words.push_back(line);
    }
    wordfile.close();

    isRunning = true;
  } else {
    printf("Some init failed: %s\n", SDL_GetError());
    isRunning = false;
  }
}

Game::~Game() {
  for (int i = 0; i < gameButtons.size(); i++) {
    delete gameButtons[i];
  }
  if (setTimer) {
    SDL_RemoveTimer(timerIdShowWord);
    SDL_RemoveTimer(timerIdUpdateWordsLocation);
  }
  delete wordsOnScreen;
  wordsOnScreen = NULL;
  delete player;
  player = NULL;
  SDL_FreeSurface(backgroundImage);
  SDL_DestroyTexture(backgroundTex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  Mix_FreeMusic(backgroundMusic);
  TTF_CloseFont(font);
  IMG_Quit();
  SDL_Quit();
}

void Game::updateWordsLocation(void *param) {
  Game *game = static_cast<Game *>(param);
  map<string, pair<int, int>>::const_iterator it =
      game->wordsOnScreen->cbegin();

  game->renderClear();
  while (it != game->wordsOnScreen->cend()) {
    SDL_Rect dst;

    dst.x = it->second.first;
    dst.y = it->second.second;

    int windowHeight;
    int windowWidth;

    SDL_GetWindowSize(game->window, &windowWidth, &windowHeight);

    if (it->second.second + 15 > windowHeight) {
      game->wordsOnScreen->erase(it++);

      game->player->loseLife(1);

      if (game->player->health == 0) {
#ifdef __EMSCRIPTEN__
        emscripten_clear_interval(game->timerIdShowWord);
        emscripten_clear_interval(game->timerIdUpdateWordsLocation);
        emscripten_cancel_main_loop();
#endif
      }
    } else {
      SDL_Surface *surfaceMessage = TTF_RenderUTF8_Blended(
          game->font, it->first.c_str(), game->fontColor);

      SDL_Texture *message =
          SDL_CreateTextureFromSurface(game->renderer, surfaceMessage);

      pair<int, int> newPosition =
          make_pair(it->second.first, it->second.second + 15);

      game->wordsOnScreen->operator[](it->first) = newPosition;

      TTF_SizeUTF8(game->font, it->first.c_str(), &dst.w, &dst.h);

      SDL_RenderCopy(game->renderer, message, NULL, &dst);

      SDL_FreeSurface(surfaceMessage);
      SDL_DestroyTexture(message);

      it++;
    }
  }
  game->showScore();
  game->showLives();
  game->render();
}

Uint32 Game::updateWordsLocation(Uint32 interval, void *param) {
  Game *game = static_cast<Game *>(param);
  map<string, pair<int, int>>::const_iterator it =
      game->wordsOnScreen->cbegin();

  game->renderClear();
  while (it != game->wordsOnScreen->cend()) {
    SDL_Rect dst;

    dst.x = it->second.first;
    dst.y = it->second.second;

    int windowHeight;
    int windowWidth;

    SDL_GetWindowSize(game->window, &windowWidth, &windowHeight);

    if (it->second.second + 15 > windowHeight) {
      game->wordsOnScreen->erase(it++);

      game->player->loseLife(1);

      if (game->player->health == 0) {
        game->isRunning = false;
      }
    } else {
      SDL_Surface *surfaceMessage = TTF_RenderUTF8_Blended(
          game->font, it->first.c_str(), game->fontColor);

      SDL_Texture *message =
          SDL_CreateTextureFromSurface(game->renderer, surfaceMessage);

      pair<int, int> newPosition =
          make_pair(it->second.first, it->second.second + 15);

      game->wordsOnScreen->operator[](it->first) = newPosition;

      TTF_SizeUTF8(game->font, it->first.c_str(), &dst.w, &dst.h);

      SDL_RenderCopy(game->renderer, message, NULL, &dst);

      SDL_FreeSurface(surfaceMessage);
      SDL_DestroyTexture(message);

      it++;
    }
  }
  game->showScore();
  game->showLives();
  game->render();

  return interval;
}

bool Game::canAddWord() {
  if (difficulty == EASY && wordsOnScreen->size() < 10) {
    return true;
  } else if (difficulty == MEDIUM && wordsOnScreen->size() < 20) {
    return true;
  } else if (difficulty == HARD && wordsOnScreen->size() < 30) {
    return true;
  }
  return false;
}

void Game::showWord(void *param) {
  Game *game = static_cast<Game *>(param);
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
}

Uint32 Game::showWord(Uint32 interval, void *param) {
  Game *game = static_cast<Game *>(param);
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
  while (SDL_PollEvent(&event)) {
    for (int i = 0; i < gameButtons.size(); i++) {
      gameButtons[i]->handleEvents(event);
    }
    string newChar = "";
#ifdef __EMSCRIPTEN__
    bool isBackspace = event.key.keysym.sym == 42;
    bool isEnter = event.key.keysym.sym == 40;
    newChar = event.key.keysym.scancode;
#else
    bool isBackspace = event.key.keysym.sym == SDLK_BACKSPACE;
    bool isEnter = event.key.keysym.sym == SDLK_RETURN;
    newChar = event.text.text;
#endif
    switch (event.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (isBackspace && wordTyping.size() > 0) {
        wordTyping.pop_back();
      }
      if (isEnter) {
        if (isWordTypingOnScreen()) {
          removeWord();
          if (difficulty == EASY) {
            addScore(1);
          } else if (difficulty == MEDIUM) {
            addScore(3);
          } else if (difficulty == HARD) {
            addScore(5);
          }
        }
        wordTyping.clear();
      }
      break;
    case SDL_TEXTINPUT:
      wordTyping.append(newChar);
      break;
    }
  }
}

void Game::addScore(int sumScore) { score += sumScore; }

void Game::showScore() {
  stringstream ss;
  ss << "Points: " << score;
  SDL_Surface *surface =
      TTF_RenderUTF8_Blended(font, ss.str().c_str(), fontColor);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect dst;
  TTF_SizeUTF8(font, ss.str().c_str(), &dst.w, &dst.h);
  dst.x = SCREEN_WIDTH - 150;
  dst.y = SCREEN_HEIGHT - 100;

  SDL_RenderCopy(renderer, texture, NULL, &dst);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void Game::showLives() {
  stringstream ss;
  ss << "Lives: " << player->health;
  SDL_Surface *surface =
      TTF_RenderUTF8_Blended(font, ss.str().c_str(), fontColor);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect dst;
  TTF_SizeUTF8(font, ss.str().c_str(), &dst.w, &dst.h);
  dst.x = SCREEN_WIDTH - 150;
  dst.y = SCREEN_HEIGHT - 50;

  SDL_RenderCopy(renderer, texture, NULL, &dst);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void Game::render() { SDL_RenderPresent(renderer); }

void Game::renderClear() { SDL_RenderClear(renderer); }

void Game::addButton(const char *text, function<void()> *fn, SDL_Rect *dst) {
  Button *button = new Button(text, fontColor, font, dst, renderer, fn);
  gameButtons.push_back(button);
}

void Game::addText(const char *text, SDL_Rect *dst) {
  SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, fontColor);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderCopy(renderer, texture, NULL, dst);
}

bool Game::running() { return isRunning; }
