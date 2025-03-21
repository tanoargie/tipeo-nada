#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_timer.h>

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#include "../Audio/Audio.cpp"
#include "../Button/Button.cpp"
#include "../Graphics/Graphics.cpp"
#include "../Player/Player.cpp"
#include "../constants.cpp"

using namespace std;

class Game {

public:
  Game();
  ~Game();

  difficulty difficulty = difficulty::NOT_SET;
  bool sessionEnded = false;
  Uint32 timerIdShowWord = 0;
  Uint32 timerIdUpdateWordsLocation = 0;

  void handleEvents();
  void removeWord();
  bool isWordTypingOnScreen();
  void showScore();
  void showLives();
  bool canAddWord();
  void askForRetry();
  void resetScore();
  bool running();
  void showMenu();
  void showRetryMenu();
  void gameLoop();
  SDL_Rect addButton(const char *text, int x, int y, function<void()> *fn,
                     position position);
  void addScore(int sumScore);

  void showWord();
  void updateWordsLocation();

  static Uint32 showWord(Uint32 interval, void *param);
  static void showWord(void *param);

  static Uint32 updateWordsLocation(Uint32 interval, void *param);
  static void updateWordsLocation(void *param);

private:
  int score = 0;

  vector<Button *> gameButtons;
  string wordTyping;
  Player *player;
  Audio *audio;
  Graphics *graphics;
  map<string, pair<int, int>> *wordsOnScreen;
  vector<string> words;
  bool isRunning;
};

#endif
