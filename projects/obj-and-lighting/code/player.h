#pragma once
#include "render/GraphicsNode.h"
#include "entity.h"

class Map;

struct Player : Entity
{
    float moveSpeed = 0.2f;
    float radius;
    GraphicsNode* gNode;
    Vector aimDir;
    float range = 10.f;
    const long int shootingRate = 50; // coolDown effect
    long int shootingTime = -1; // in seconds currently
public:
    Player();
    ~Player();
    void Shoot(Map* map);
};
