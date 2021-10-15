#include "player.h"

Player::Player(bool alive=true) : alive(alive)
{
    position = vec3(0, 0, 0);
    rotation = vec3(0, 0, 0);
}

Player::~Player()
{
    
}
