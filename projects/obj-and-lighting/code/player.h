#pragma once
#include "entity.h"

struct Player : Entity
{
    float moveSpeed = 0.2f;
    float radius;
    float velocity;

public:
    Player();
    ~Player();
};
