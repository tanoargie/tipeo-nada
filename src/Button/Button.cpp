#ifndef Button_cpp
#define Button_cpp

#include <SDL.h>

#include <functional>

using namespace std;

class Button {
public:
  Button(function<void()> *fn, SDL_Rect dst) : dst(dst) {
    callback = new function<void()>;
    *callback = *fn;
  };

  void handleEvents(SDL_Event event) {
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN:
      if (mouseInsideDiv()) {
        (*callback)();
      }
      break;
    }
  };

  bool mouseInsideDiv() {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (x > dst.x && x < dst.x + dst.w && y > dst.y && y < dst.y + dst.h) {
      return true;
    }
    return false;
  }

  ~Button() {
    delete callback;
    callback = NULL;
  };

private:
  SDL_Rect dst;
  function<void()> *callback;
};

#endif
