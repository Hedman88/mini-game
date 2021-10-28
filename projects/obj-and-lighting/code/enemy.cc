#include "enemy.h"
#include <ctime>
#include <cmath>

Enemy::Enemy(Vector position)
{
    this->position = position;
    this->graphicNode = new GraphicsNode();
}

Enemy::~Enemy()
{
    
}



// spawn waves onto map
void Enemy::SpawnEnemies(std::vector<Enemy>* enemies, unsigned int waves, const unsigned int width, const unsigned int height)
{
    if (enemies == nullptr)
    {
        return;
    }
    if (enemies->size() > 0)
    {
        return;
    }

    srand((unsigned int)time(0));

    for (size_t i = 0; i < 5 + (int)(waves * 1.09f); i++)
    {
        int x = rand() % width, z = rand() % height;
        Vector position = Vector(x, 0, z);
        Enemy en(position);
        enemies->push_back(en);
    }
}

void Enemy::Update(Player player)
{
    velocity = player.position - position;
    if (velocity.Length())
    velocity.Normalize();
    position = position + velocity * moveSpeed;
}
