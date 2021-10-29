#pragma once
#include "entity.h"
#include "player.h"
#include "core/mathLib.h"
#include "render/GraphicsNode.h"
#include <vector>

class Score;
class Map;
struct Enemy : Entity
{
    float moveSpeed = 0.1f;
    float radius = 0.01f;
    std::shared_ptr<GraphicsNode> graphicNode;

    Score* score;

    void MoveTowardsPlayer(Player* player);
    static void SpawnEnemies(std::vector<Enemy>* enmemies, Map* map, unsigned int waves, const unsigned int width, const unsigned int height);

    void Update(Player player);
    Enemy(Vector position);
    ~Enemy();
};
