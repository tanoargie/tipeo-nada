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
#include <sstream>
#include <string>
#include <time.h>
#include <utility>
#include <vector>

#include "../Button/Button.cpp"
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
  Uint32 timerIdShowWord = 0;
  Uint32 timerIdUpdateWordsLocation = 0;

  bool setTimer = false;
  void handleEvents();
  void render();
  void renderClear();
  void draw();
  void removeWord();
  bool isWordTypingOnScreen();
  static Uint32 showWord(Uint32 interval, void *param);
  static void showWord(void *param);
  static Uint32 updateWordsLocation(Uint32 interval, void *param);
  static void updateWordsLocation(void *param);
  void showScore();
  void showLives();
  bool initializeAudio();

  bool running();
  bool canAddWord();
  void addButton(const char *text, function<void()> *fn, SDL_Rect *dst);
  void addText(const char *text, SDL_Rect *dst);
  void addScore(int sumScore);

private:
  SDL_Surface *backgroundImage;
  SDL_Texture *backgroundTex;
  int score = 0;
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
