#pragma once
#include "render/GraphicsNode.h"
#include "entity.h"
#include <vector>

class Map;
class Enemy;

struct Player : Entity
{
    float moveSpeed = 0.05f;
    float radius;
    GraphicsNode* gNode;
    Vector aimDir;
    float range = 10.f;
    const long int shootingRate = 50; // coolDown effect
    long int shootingTime = -1; // in seconds currently
public:
    bool shotFired = false;
    Player();
    ~Player();
    void Shoot(Map* map, std::vector<Enemy>* enemies);
};
