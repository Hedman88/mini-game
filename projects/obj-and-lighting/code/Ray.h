#pragma once
#include "core/mathLib.h"
#include "enemy.h"
#include "Map.h"

class Ray
{
    Vector pos;
    Vector dir;
    int range;
    Enemy *hitEnemy;
  public:
    Ray(Vector position, Vector direction, int range);
    Ray(Vector position, Vector direction, int range, Map* map, std::vector<Enemy>* enemies);
    bool LookForEnemies(std::shared_ptr<Tile> tile);
    bool CheckEnemyIntersection(Vector position, Vector direction, std::shared_ptr<Tile> tile, std::vector<Enemy>* enemies);
};
