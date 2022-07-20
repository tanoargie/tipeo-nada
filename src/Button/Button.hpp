#ifndef Button_hpp
#define Button_hpp

#include "../constants.cpp"
#include "SDL_mouse.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_ttf.h>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

class Button {
public:
  Button(const char *text, SDL_Color fontColor, TTF_Font *font, SDL_Rect *dst,
         SDL_Renderer *renderer, function<void()> *fn)
      : font(font), fontColor(fontColor), dst(dst) {

    callback = new function<void()>;
    *callback = *fn;

    TTF_SizeUTF8(font, text, &(dst->w), &(dst->h));
    surface = TTF_RenderUTF8_Blended(font, text, fontColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, NULL, dst);
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

    if (x > dst->x && x < dst->x + dst->w && y > dst->y &&
        y < dst->y + dst->h) {
      return true;
    }
    return false;
  }

  ~Button() {
    delete callback;
    callback = NULL;
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
  };

private:
  bool selected;
  SDL_Texture *texture;
  SDL_Surface *surface;
  SDL_Color fontColor;
  SDL_Rect *dst;
  TTF_Font *font;
  function<void()> *callback;
  char *text;
};

#endif
