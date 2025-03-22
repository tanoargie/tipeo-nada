cmake_minimum_required(VERSION 3.15)

project(TipeoNada VERSION 1.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_FLAGS "-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='[\"png\"]' -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 -sUSE_MPG123=1 -sSDL2_MIXER_FORMATS='[\"mp3\"]'")

  set( SDL_TEST OFF CACHE BOOL "" FORCE )
  add_subdirectory("external/SDL")

  set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
  set( SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE )
  set( SDL2IMAGE_VENDORED OFF CACHE BOOL "" FORCE )
  set( SDL2IMAGE_AVIF OFF CACHE BOOL "" FORCE )
  add_subdirectory("external/SDL_image")

  set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
  set( SDL2TTF_HARFBUZZ OFF CACHE BOOL "" FORCE )
  set( SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE )
  set( SDL2TTF_VENDORED ON CACHE BOOL "" FORCE )
  add_subdirectory("external/SDL_ttf")

  set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_SAMPLES OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE )
  set( SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_MOD OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_OPUS OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_INSTALL OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_DEPS_SHARED OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_FLAC OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_MOD OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_MP3 ON CACHE BOOL "" FORCE )
  set( SDL2MIXER_MIDI OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_OPUS OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE )
  add_subdirectory("external/SDL_mixer")

  add_executable(${PROJECT_NAME}Web src/main.cpp src/Game/Game.cpp src/Player/Player.cpp src/Button/Button.cpp src/Graphics/Graphics.cpp src/Font/Font.cpp src/Audio/Audio.cpp)

  set_target_properties(${PROJECT_NAME}Web PROPERTIES LINK_FLAGS "-sALLOW_MEMORY_GROWTH -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='[\"png\"]' -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 -sUSE_MPG123=1 -sSDL2_MIXER_FORMATS='[\"mp3\"]' -lmpg123 --preload-file ../src/Assets")
else()
  set( SDL2MIXER_SAMPLES OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_INSTALL OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE )
  set( SDL2MIXER_DEPS_SHARED OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_FLAC OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_MOD OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_MP3 ON CACHE BOOL "" FORCE )
  set( SDL2MIXER_MIDI OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_OPUS OFF CACHE BOOL "" FORCE )
  set( SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE )

  add_executable(${PROJECT_NAME} src/main.cpp src/Game/Game.cpp src/Player/Player.cpp src/Button/Button.cpp src/Graphics/Graphics.cpp src/Font/Font.cpp src/Audio/Audio.cpp)

  add_subdirectory("external/SDL")
  target_link_libraries(${PROJECT_NAME} SDL2)

  add_subdirectory("external/SDL_image")
  target_link_libraries(${PROJECT_NAME} SDL2_image)

  add_subdirectory("external/SDL_ttf")
  target_link_libraries(${PROJECT_NAME} SDL2_ttf)

  add_subdirectory("external/SDL_mixer")
  target_link_libraries(${PROJECT_NAME} SDL2_mixer)

  file(COPY src/Assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

