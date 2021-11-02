#pragma once
#include "entity.h"
#include "player.h"
#include "core/mathLib.h"
#include "render/GraphicsNode.h"
#include <vector>

class Score;
class Map;
class Tile;
struct Enemy : Entity
{
    float moveSpeed = 0.01f;
    float radius = 0.1f;
    std::shared_ptr<GraphicsNode> graphicNode;
    std::vector<std::shared_ptr<Tile>> currentTiles;
    int ID = -1;
    bool dead = false;

    Score* score;

    void MoveTowardsPlayer(Player* player);
    static void SpawnEnemies(std::vector<Enemy>* enemies, Map* map, unsigned int waves, const unsigned int width, const unsigned int height);

    void Update(Player player, Map* map);
    void Kill();
    void UpdateX(Player player);
    void UpdateZ(Player player);
    void UpdateTiles(Map* map);
    bool CheckIfTilesContain(std::shared_ptr<Tile> tile);
    Enemy(Vector position);
    ~Enemy();
};
