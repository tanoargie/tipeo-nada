#ifndef Player_hpp
#define Player_hpp

class Player {
public:
  Player();
  void setHealth(int h) { health = h; }
  void setMaxHealth(int h) { maxHealth = h; }
  int getHealth() const { return health; }
  int getMaxHealth() const { return maxHealth; }

private:
  int health, maxHealth;
};
#endif
