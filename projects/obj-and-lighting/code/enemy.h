#pragma once
#include "entity.h"
#include "player.h"
#include <vector>

struct Enemy : Entity
{
    float velocity;
    float acceleration;
    bool alive;
public:
    void moveTowardsPlayer(Player* player);
    static void spawnEnemies(std::vector<Enemy*>* enmemies, unsigned int waves, const unsigned int width, const unsigned int height);

    Enemy(Vector position, Vector rotation, bool alive);
    ~Enemy();
};