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
    void MoveTowardsPlayer(Player* player);
    static void SpawnEnemies(std::vector<Enemy>* enmemies, unsigned int waves, const unsigned int width, const unsigned int height);

    void Update(Player* player);
    Enemy(Vector position, Vector rotation, bool alive);
    ~Enemy();
};