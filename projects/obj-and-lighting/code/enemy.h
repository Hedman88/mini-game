#pragma once
#include "entity.h"
#include "player.h"
#include "core/mathLib.h"
#include "render/GraphicsNode.h"
#include <vector>

struct Enemy : Entity
{
    float moveSpeed = 0.1f;
    Vector velocity;
    GraphicsNode* graphicNode;
public:
    void MoveTowardsPlayer(Player* player);
    static void SpawnEnemies(std::vector<Enemy>* enmemies, unsigned int waves, const unsigned int width, const unsigned int height);

    void Update(Player player);
    Enemy(Vector position);
    ~Enemy();
};