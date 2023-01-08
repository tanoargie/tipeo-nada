# TipeoNada

#### This is a game in C++ based on the SDL framework to test your typing skills

###### Libraries required to develop
- SDL
- SDL_Mixer
- SDL_Image
- SDL_TTF
- Emscripten (EMCC)

It uses [Emscripten](https://emscripten.org/) to compile it to an html and play it on the web.

For both versions (web and desktop) [CMake](https://cmake.org/) is used. To build the web version it should be enough to do:
```
emcmake cmake -B build_emscripten
cd build_emscripten
make
```
and for the desktop version:
```
cmake -B build
cd build
make
```

You can add any word corpus to the game in `src/Assets/words.txt`. Be careful because Emscripten (the web version) doesn't work very well with composite keys (E.G.: è, á, etc.).

You can check it out and play it online [here](https://storage.googleapis.com/tipeo-nada-web/TipeoNadaWeb.html)

Some screenshots:

![menu](screenshots/menu.png)
![gameplay](screenshots/gameplay.png)
