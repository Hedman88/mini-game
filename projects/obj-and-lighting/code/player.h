#pragma once
#include "entity.h"

struct Player : Entity
{
    float velocity;
    float rotation;
    bool alive;

public:
    Player();
    ~Player();
};
