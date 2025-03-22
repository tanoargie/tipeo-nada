#ifndef Audio_hpp
#define Audio_hpp

#include <SDL.h>
#include <SDL_mixer.h>

class Audio {
public:
  Audio() {
    backgroundMusic = Mix_LoadMUS("../src/Assets/cyberpunk.mp3");
    if (!backgroundMusic) {
      printf("Error loading music: %s\n", Mix_GetError());
    }
    Mix_VolumeMusic((20 * MIX_MAX_VOLUME) / 100);
    if (Mix_PlayMusic(backgroundMusic, -1) < 0) {
      printf("Error playing music: %s\n", Mix_GetError());
    }
  }
  ~Audio() {
    Mix_FreeMusic(backgroundMusic);
    backgroundMusic = NULL;
  }

private:
  Mix_Music *backgroundMusic;
};
#endif
