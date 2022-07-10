#ifndef Game_hpp
#define Game_hpp

#include "SDL_events.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_keycode.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

#include "../Player/Player.hpp"

using namespace std;

class Game {

public:
  Game(const char *title, int width, int height);
  ~Game();

  void handleEvents();
  void render();
  void draw();
  static Uint32 showWord(Uint32 interval, void *param);

  bool running();

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Surface *backgroundImage;
  SDL_Texture *backgroundTex;
  SDL_Color fontColor;
  TTF_Font *font;
  vector<string> words;
  bool isRunning;
  Player player;
};

#endif
