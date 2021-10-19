#include "enemy.h"
#include <ctime>
#include <cmath>

Enemy::Enemy(Vector position, Vector rotation, bool alive=true) : alive(alive)
{
    this->position = position;
    this->rotation = rotation;
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
        int x = rand() % width, y = rand() % height;
        Vector position = Vector(x, y, 0);
        Enemy en(position, Vector(0, 0, 0), true);
        enemies->push_back(en);
    }
}

void Enemy::Update(Player* player)
{
    if (player == nullptr)
        return;
    if (!this->alive)
        return;
    acceleration = player->position - position;
    acceleration.Normalize();

    velocity = (acceleration - velocity) * .5f;
    velocity.Normalize();

    position = position + velocity;
}