#pragma once
#include "core/mathLib.h"
#include "enemy.h"

class Ray
{
    Vector pos;
    Vector dir;
    int range;
    Enemy *hitEnemy;
  public:
    Ray(Vector position, Vector direction, int range);
};
