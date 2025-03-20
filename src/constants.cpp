#ifndef constants_cpp
#define constants_cpp

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

enum class difficulty { NOT_SET, EASY, MEDIUM, HARD };
enum class position { NOT_SET, TOP, MIDDLE, BOTTOM };

static const char *difficultyToString(difficulty difficulty) {
  switch (difficulty) {
  case difficulty::EASY:
    return "EASY";
  case difficulty::MEDIUM:
    return "MEDIUM";
  case difficulty::HARD:
    return "HARD";
  default:
    return "NOT_SET";
    break;
  }
};

#endif
