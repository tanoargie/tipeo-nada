#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <time.h>
#include <utility>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#include "../Button/Button.cpp"
#include "../Player/Player.hpp"
#include "../constants.cpp"

using namespace std;

class Game {

public:
  Game();
  ~Game();

  difficultyEnum difficulty = NOT_SET;
  bool sessionEnded = false;
  Uint32 timerIdShowWord = 0;
  Uint32 timerIdUpdateWordsLocation = 0;

  SDL_Color fontColor;
  TTF_Font *font;
  Mix_Music *backgroundMusic;

  void handleEvents();
  void render();
  void renderClear();
  void removeWord();
  bool isWordTypingOnScreen();
  void showScore();
  void showLives();
  bool initializeAudio();
  bool running();
  bool canAddWord();
  void askForRetry();
  void resetScore();
  void addButton(const char *text, function<void()> *fn, SDL_Rect *dst);
  void addText(const char *text, SDL_Rect *dst);
  void addScore(int sumScore);

  void showWord();
  void updateWordsLocation();

  static Uint32 showWord(Uint32 interval, void *param);
  static void showWord(void *param);
  static Uint32 updateWordsLocation(Uint32 interval, void *param);
  static void updateWordsLocation(void *param);

private:
  int score = 0;

  SDL_Surface *backgroundImage;
  SDL_Texture *backgroundTex;
  vector<Button *> gameButtons;
  string wordTyping;
  SDL_Window *window;
  Player *player;
  SDL_Renderer *renderer;
  map<string, pair<int, int>> *wordsOnScreen;
  vector<string> words;
  bool isRunning;
};

#endif
