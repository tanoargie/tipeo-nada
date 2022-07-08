#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "../Player/Player.hpp"

using namespace std;

class Game {

public:
  Game(const char *title, int width, int height);
  ~Game();

  void handleEvents();
  void update();
  void render();
  void draw();

  bool running();

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Surface *backgroundImage;
  SDL_Texture *backgroundTex;
  bool isRunning;
  Player player;
};

#endif
