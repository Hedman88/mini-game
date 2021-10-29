#include "enemy.h"
#include "Map.h"
#include <ctime>
#include <cmath>

Enemy::Enemy(Vector position)
{
    this->position = position;
    this->graphicNode = std::make_shared<GraphicsNode>();
}

Enemy::~Enemy()
{
    Die();
}

void Enemy::Die()
{
    // score
}

// spawn waves onto map
void Enemy::SpawnEnemies(std::vector<Enemy>* enemies, Map* map, unsigned int waves, const unsigned int width, const unsigned int height)
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
        int x, z;
        do
        {
            x = rand() % width, z = rand() % height;
            if (map->GetTile(x, z)->walkable) break;
        }while(true);
        Vector position = Vector(x, 0, z);
        Enemy en(position);
        enemies->push_back(en);
    }
}

void Enemy::Update(Player player)
{
    Vector temp;
    temp = player.position - this->position;
    if (temp.Length())
        temp.Normalize();
    position = position + temp * moveSpeed;
}
