#include "Game.hpp"
#include "SDL_events.h"

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
  IMG_Quit();
  SDL_Quit();
}

void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    isRunning = false;
    break;
  default:
    break;
  }
}

void Game::update() {}

void Game::render() {
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, backgroundTex, NULL, NULL);
  SDL_RenderPresent(renderer);
}

bool Game::running() { return isRunning; }
