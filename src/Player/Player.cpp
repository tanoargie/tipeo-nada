#ifndef Player_hpp
#define Player_hpp

class Player {
public:
  Player() { health = 5; }
  ~Player() {}

  bool isAlive() { return health > 0; };
  void loseLife(int lostLife) { health -= lostLife; };
  void resetLifes() { health = 5; };
  int getLife() { return health; }

private:
  int health;
};
#endif
