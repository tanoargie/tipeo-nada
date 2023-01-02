project(TipeoNada)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_STANDARD 14)

cmake_minimum_required(VERSION 3.15)

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  add_executable(${PROJECT_NAME}Web src/main.cpp src/Game/Game.cpp src/Player/Player.cpp src/Button/Button.cpp)

  set_target_properties(${PROJECT_NAME}Web PROPERTIES LINK_FLAGS "-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='[\"png\"]' -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 -sUSE_MPG123=1 -sSDL2_MIXER_FORMATS='[\"mp3\"]' -lmpg123 --preload-file ../src/Assets")
else()
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_ttf REQUIRED)
  find_package(SDL2_mixer REQUIRED)
  
  add_executable(${PROJECT_NAME} src/main.cpp src/Game/Game.cpp src/Player/Player.cpp src/Button/Button.cpp)
  
  include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
  
  target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
endif()

