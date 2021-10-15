#include "player.h"

Player::Player(bool alive=true) : alive(alive)
{
    position = Vector(0, 0, 0);
    rotation = Vector(0, 0, 0);
}

Player::~Player()
{
    
}
