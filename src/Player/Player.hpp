#ifndef Player_hpp
#define Player_hpp

class Player {
public:
  Player();

  int health = 5;

  void loseLife(int lostLife);
  void resetLifes();
};
#endif
