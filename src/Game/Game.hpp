#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <time.h>
#include <utility>
#include <vector>

#include "../Button/Button.hpp"
#include "../Player/Player.hpp"
#include "../constants.cpp"

using namespace std;

class Game {

public:
  Game(const char *title);
  ~Game();

  difficultyEnum difficulty = NOT_SET;

  SDL_Color fontColor;
  TTF_Font *font;
  Mix_Music *backgroundMusic;

  void handleEvents();
  void render();
  void renderClear();
  void draw();
  void removeWord();
  bool isWordTypingOnScreen();
  static Uint32 showWord(Uint32 interval, void *param);
  static Uint32 updateWordsLocation(Uint32 interval, void *param);

  bool running();
  bool canAddWord();
  void addButton(const char *text, function<void()> *fn, SDL_Rect *dst);
  void addText(const char *text, SDL_Rect *dst);

private:
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Surface *backgroundImage;
  SDL_Texture *backgroundTex;
  vector<string> words;
  map<string, pair<int, int>> *wordsOnScreen;
  bool isRunning;
  string wordTyping;
  Player *player;
  vector<Button *> gameButtons;
};

#endif
