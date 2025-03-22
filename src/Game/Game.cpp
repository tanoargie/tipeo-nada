#include "Game.hpp"

mutex wordsOnScreenMutex;

#ifdef __EMSCRIPTEN__
void one_iter_session_ended(void *userData) {
  Game *game = static_cast<Game *>(userData);
  if (game->sessionEnded) {
    game->showRetryMenu();    
  }
}
#endif


Game::Game() {
  if (SDL_Init(SDL_INIT_TIMER) == 0 && IMG_Init(IMG_INIT_PNG) &&
      TTF_Init() == 0 && Mix_Init(MIX_INIT_MP3) != 0 &&
      Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0) {
    printf("Initialized!");
#ifdef __EMSCRIPTEN__
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
#endif
    graphics = new Graphics();
    audio = new Audio();
    player = new Player();

    if (!graphics || !audio || !player) {
      isRunning = false;
    }

    wordsOnScreen = new map<string, pair<int, int>>();

    srand(time(0));

    ifstream wordfile("../src/Assets/words.txt");
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

void Game::showMenu() {
  function<void()> setEasyDifficulty = [&]() { difficulty = difficulty::EASY; };
  function<void()> setMediumDifficulty = [&]() { difficulty = difficulty::MEDIUM; };
  function<void()> setHardDifficulty = [&]() { difficulty = difficulty::HARD; };

  const char *diffMessage = "Choose difficulty, please:";
  SDL_Rect diffRect = graphics->drawText(diffMessage, 0, -100, position::MIDDLE);
  addButton(
    difficultyToString(difficulty::EASY), 
    0, 
    100,
    &setEasyDifficulty,
    position::MIDDLE
  );
  addButton(
    difficultyToString(difficulty::MEDIUM), 
    0, 
    150,
    &setMediumDifficulty,
    position::MIDDLE
  );
  addButton(
    difficultyToString(difficulty::HARD), 
    0, 
    200,
    &setHardDifficulty,
    position::MIDDLE
  );
}

void Game::showRetryMenu() {
  function<void()> sayNo = [&]() {
    isRunning = false;
    sessionEnded = false;
  };

  function<void()> sayYes = [&]() {
    player->resetLifes();
    sessionEnded = false;
    difficulty = difficulty::NOT_SET;
    resetScore();
    wordsOnScreenMutex.lock();
    wordsOnScreen->clear();
    wordsOnScreenMutex.unlock();
    gameLoop();
  };
  
  graphics->renderClear();
  const char *retryMessage = "Want to retry?";
  graphics->drawText(retryMessage, 0, -100, position::MIDDLE);
  addButton("YES", 0, 100, &sayYes, position::MIDDLE);
  addButton("NO", 0, 50, &sayNo, position::MIDDLE);
  graphics->render();
}

bool Game::running() { return isRunning; }

void Game::gameLoop() {
  if (!sessionEnded && difficulty == difficulty::NOT_SET) {
    graphics->renderClear();
    showMenu();
    graphics->render();
  } else if (!sessionEnded) {
    if (timerIdShowWord == 0 || timerIdUpdateWordsLocation == 0) {
      if (difficulty == difficulty::EASY) {
        timerIdShowWord = SDL_AddTimer(3000, &showWord, this);
      } else if (difficulty == difficulty::MEDIUM) {
        timerIdShowWord = SDL_AddTimer(1500, &showWord, this);
      } else if (difficulty == difficulty::HARD) {
        timerIdShowWord = SDL_AddTimer(500, &showWord, this);
      }
      timerIdUpdateWordsLocation =
          SDL_AddTimer(250, &updateWordsLocation, this);
    }
  }
}

Game::~Game() {
  for (int i = 0; i < gameButtons.size(); i++) {
    delete gameButtons[i];
  }
  delete graphics;
  delete audio;
  delete player;
  SDL_RemoveTimer(timerIdShowWord);
  SDL_RemoveTimer(timerIdUpdateWordsLocation);
  delete wordsOnScreen;
  wordsOnScreen = NULL;
  IMG_Quit();
  SDL_Quit();
}

void Game::resetScore() { score = 0; }

void Game::askForRetry() {
#ifdef __EMSCRIPTEN__
  emscripten_clear_interval(timerIdShowWord);
  emscripten_clear_interval(timerIdUpdateWordsLocation);
  emscripten_cancel_main_loop();
#else
  bool showWordRemoved = SDL_RemoveTimer(timerIdShowWord);
  bool updateWordsRemoved = SDL_RemoveTimer(timerIdUpdateWordsLocation);
  if (updateWordsRemoved && showWordRemoved) {
    timerIdShowWord = 0;
    timerIdUpdateWordsLocation = 0;
  }
#endif
  sessionEnded = true;
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(one_iter_session_ended, this, 0, 1);
#else
  while (sessionEnded) {
    showRetryMenu();
  }
#endif
}

void Game::updateWordsLocation(void *param) {
  Game *game = static_cast<Game *>(param);
  wordsOnScreenMutex.lock();
  game->updateWordsLocation();
  wordsOnScreenMutex.unlock();
}

Uint32 Game::updateWordsLocation(Uint32 interval, void *param) {
  Game *game = static_cast<Game *>(param);
  wordsOnScreenMutex.lock();
  game->updateWordsLocation();
  wordsOnScreenMutex.unlock();

  return interval;
}

void Game::updateWordsLocation() {
  map<string, pair<int, int>>::const_iterator it = wordsOnScreen->cbegin();

  graphics->renderClear();
  while (!wordsOnScreen->empty() && it != wordsOnScreen->cend()) {
    int windowHeight;
    int windowWidth;

    graphics->getWindowSize(&windowWidth, &windowHeight);

    if (it->second.second + 15 > windowHeight) {
      wordsOnScreen->erase(it++);

      player->loseLife(1);

      if (!player->isAlive()) {
        return askForRetry();
      }
    } else {
      pair<int, int> newPosition =
          make_pair(it->second.first, it->second.second + 15);

      if (wordsOnScreen->find(it->first) != wordsOnScreen->end()) {
        wordsOnScreen->operator[](it->first) = newPosition;
      }

      graphics->drawText(it->first.c_str(), it->second.first,
                         it->second.second);

      it++;
    }
  }
  showScore();
  showLives();
  graphics->render();
}

bool Game::canAddWord() {
  if (difficulty == difficulty::EASY) {
    return wordsOnScreen->size() < 10;
  } else if (difficulty == difficulty::MEDIUM) {
    return wordsOnScreen->size() < 20;
  } else if (difficulty == difficulty::HARD) {
    return wordsOnScreen->size() < 30;
  }
  return false;
}

Uint32 Game::showWord(Uint32 interval, void *param) {
  Game *game = static_cast<Game *>(param);
  game->showWord();

  return interval;
}

void Game::showWord(void *param) {
  Game *game = static_cast<Game *>(param);
  game->showWord();
}

void Game::showWord() {
  int random = rand() % words.size();
  int randomX = rand() % (SCREEN_WIDTH - 100);

  string randomWord = words.at(random);
  if (randomX > 1100 && randomWord.size() > 5) {
    randomX -= 300;
  }

  pair<int, int> position = make_pair(randomX, 0);

  if (canAddWord()) {
    wordsOnScreen->insert(pair<string, pair<int, int>>(randomWord, position));
  }
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
#ifdef __EMSCRIPTEN__
    bool isBackspace = event.key.keysym.scancode == 42;
    bool isEnter = event.key.keysym.scancode == 40;
#else
    bool isBackspace = event.key.keysym.sym == SDLK_BACKSPACE;
    bool isEnter = event.key.keysym.sym == SDLK_RETURN;
#endif
    string newChar = event.text.text;
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
          if (difficulty == difficulty::EASY) {
            addScore(1);
          } else if (difficulty == difficulty::MEDIUM) {
            addScore(3);
          } else if (difficulty == difficulty::HARD) {
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
  string text = string("Points: ") + to_string(score);
  graphics->drawText(text.c_str(), SCREEN_WIDTH - 150, SCREEN_HEIGHT - 100);
}

void Game::showLives() {
  string text = string("Lives: ") + to_string(player->getLife());
  graphics->drawText(text.c_str(), SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50);
}

SDL_Rect Game::addButton(const char *text, int x, int y, function<void()> *fn,
                         position position = position::NOT_SET) {
  SDL_Rect dst = graphics->drawText(text, x, y, position);
  Button *button = new Button(fn, dst);
  gameButtons.push_back(button);
  return dst;
}