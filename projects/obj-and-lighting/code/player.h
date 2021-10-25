#pragma once
#include "render/GraphicsNode.h"
#include "entity.h"

struct Player : Entity
{
    float moveSpeed = 0.2f;
    float radius;
    GraphicsNode* gNode;
public:
    Player();
    ~Player();
};
