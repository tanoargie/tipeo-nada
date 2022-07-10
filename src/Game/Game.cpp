#include "Game.hpp"
#include "../constants.cpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_render.h"
#include "SDL_timer.h"

Game::Game(const char *title, int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && IMG_Init(IMG_INIT_PNG)) {
    cout << "Initialized!" << endl;
    window = SDL_CreateWindow("TipeoNada", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_OPENGL);

    if (!window) {
      printf("Could not create window: %s\n", SDL_GetError());
      isRunning = false;
      return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
      printf("Could not create renderer: %s\n", SDL_GetError());
      isRunning = false;
      return;
    }

    font = TTF_OpenFont("OpenSans-Bold.ttf", 24);
    fontColor.r = 255;
    fontColor.g = 255;
    fontColor.b = 255;

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
  SDL_FreeSurface(backgroundImage);
  SDL_DestroyTexture(backgroundTex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  IMG_Quit();
  SDL_Quit();
}

Uint32 Game::showWord(Uint32 interval, void *param) {
  Game *game = reinterpret_cast<Game *>(param);
  int random = rand() % game->words.size();
  string randomWord = game->words.at(random);
  /* cout << random << " " << randomWord << endl; */

  /* backgroundImage = TTF_RenderText_Solid(font, randomWord.c_str(),
   * fontColor); */

  /* backgroundTex = SDL_CreateTextureFromSurface(renderer, backgroundImage); */
  /* int randomX = rand() % SCREEN_WIDTH; */

  return interval;

  /* SDL_RenderCopy(SDL_Renderer *renderer, message, const SDL_Rect *srcrect,
   * const SDL_Rect *dstrect); */
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
    }
    break;
  default:
    break;
  }
}

void Game::render() {
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, backgroundTex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

bool Game::running() { return isRunning; }
