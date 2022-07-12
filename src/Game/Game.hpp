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
#include <map>
#include <string>
#include <time.h>
#include <utility>
#include <vector>

#include "../Player/Player.hpp"
#include "../constants.cpp"

using namespace std;

class Game {

public:
  Game(const char *title, int width, int height);
  ~Game();

  void handleEvents();
  void render();
  void draw();
  static Uint32 showWord(Uint32 interval, void *param);
  static Uint32 updateWordsLocation(Uint32 interval, void *param);

  bool running();
  bool canAddWord();

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Surface *backgroundImage;
  SDL_Texture *backgroundTex;
  SDL_Color fontColor;
  TTF_Font *font;
  vector<string> words;
  map<string, pair<int, int>> *wordsOnScreen;
  bool isRunning;
  difficultyEnum difficulty;
  Player player;
};

#endif
