#pragma once
#include "entity.h"

struct Player : Entity
{
    float velocity;
    float acceleration;
    bool alive;

public:
    Player(bool alive);
    ~Player();
};
