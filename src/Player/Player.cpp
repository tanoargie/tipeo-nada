#include "Player.hpp"

Player::Player() {}

void Player::loseLife(int lostLife) { health -= lostLife; }

void Player::resetLifes() { health = 5; }
