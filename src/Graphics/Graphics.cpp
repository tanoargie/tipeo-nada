#ifndef Graphics_cpp
#define Graphics_cpp

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <iostream>


#include "../Font/Font.cpp"
#include "../constants.cpp"

class Graphics {
public:
  Graphics() {
    font = new Font();
    window = SDL_CreateWindow("TipeoNada", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!window || !renderer) {
      printf("Could not create graphics: %s\n", SDL_GetError());
      return;
    }
  }
  ~Graphics() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(backgroundImage);
    SDL_DestroyTexture(backgroundTex);
    renderer = NULL;
    window = NULL;
    backgroundTex = NULL;
    backgroundImage = NULL;
    delete font;
    font = NULL;
  }
  SDL_Rect drawText(const char *text, int x, int y,
                    position position = position::NOT_SET) {
    SDL_Surface *surface = font->getSurface(text);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst;
    int result = font->getRenderedString(text, &dst.w, &dst.h);
    if (result < 0) {
      printf("Could not create text: %s\n", SDL_GetError());
    }
    if (position == position::NOT_SET) {
      dst.x = x;
      dst.y = y;
    } else if (position == position::MIDDLE) {
      dst.x = SCREEN_WIDTH / 2 - (dst.w / 2) + x;
      dst.y = SCREEN_HEIGHT / 2 - (dst.h / 2) + y;
    }
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return dst;
  }
  void getWindowSize(int *width, int *height) {
    SDL_GetWindowSize(window, width, height);
  }
  void renderClear() { SDL_RenderClear(renderer); }
  void render() { SDL_RenderPresent(renderer); }

private:
  Font *font;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *backgroundImage;
  SDL_Texture *backgroundTex;
};

#endif