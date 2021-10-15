#pragma once
#include "entity.h"
#include <vector>

struct Enemy : Entity
{
    float velocity;
    float acceleration;
public:
    void moveTowardsPlayer(Player* player);
    static void spawnEnemies(const std::vector<Vector>* enmemies, unsigned int waves, const unsigned int width, const unsigned int height);
};