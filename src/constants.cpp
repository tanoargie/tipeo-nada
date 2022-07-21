#ifndef constants_cpp
#define constants_cpp

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

enum difficultyEnum { NOT_SET, EASY, MEDIUM, HARD };

static const char *getDifficultyChar(const difficultyEnum difficulty) {
  switch (difficulty) {
  case EASY:
    return "EASY";
  case MEDIUM:
    return "MEDIUM";
  case HARD:
    return "HARD";
  default:
    return "NOT_SET";
    break;
  }
};

#endif
