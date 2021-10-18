#pragma once
#include "entity.h"
#include "player.h"
#include "core/mathLib.h"
#include <vector>

struct Enemy : Entity
{
    Vector velocity;
    Vector acceleration;
    bool alive;
public:
    void moveTowardsPlayer(Player* player);
    static void spawnEnemies(std::vector<Enemy>* enmemies, unsigned int waves, const unsigned int width, const unsigned int height);

    void update();
    Enemy(Vector position, Vector rotation, bool alive);
    ~Enemy();
};