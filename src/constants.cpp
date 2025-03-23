#ifndef constants_cpp
#define constants_cpp

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 900;

enum class difficulty { NOT_SET, EASY, MEDIUM, HARD };
enum class position { NOT_SET, TOP, MIDDLE, BOTTOM };

static const char *difficultyStr(difficulty difficulty) {
  switch (difficulty) {
  case difficulty::EASY:
    return "EASY";
  case difficulty::MEDIUM:
    return "MEDIUM";
  case difficulty::HARD:
    return "HARD";
  default:
    return "NOT_SET";
  }
};

#endif
