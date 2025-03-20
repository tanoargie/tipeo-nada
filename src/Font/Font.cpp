#include <SDL.h>
#include <SDL_ttf.h>

class Font {
public:
  Font() {
    fontType = TTF_OpenFont("src/Assets/OpenSans-Bold.ttf", 24);
    if (!fontType) {
      printf("Could not create fonts: %s\n", SDL_GetError());
    }

    fontColor.r = 255;
    fontColor.g = 255;
    fontColor.b = 255;
  }
  ~Font() {
    TTF_CloseFont(fontType);
    fontType = NULL;
  }

  SDL_Surface *getSurface(const char *text) {
    return TTF_RenderUTF8_Blended(fontType, text, fontColor);
  }
  int getRenderedString(const char *text, int *w, int *h) {
    return TTF_SizeUTF8(fontType, text, w, h);
  }

private:
  SDL_Color fontColor;
  TTF_Font *fontType;
};
