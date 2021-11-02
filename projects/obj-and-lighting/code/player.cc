#include "config.h"
#include "player.h"
#include "Map.h"
#include "enemy.h"
#include "Ray.h"
#include <chrono>

Player::Player()
{
    position = Vector(0, 0, 0);
    rotation = 0.f;
}

Player::~Player()
{
    
}

void Player::Shoot(Map* map, std::vector<Enemy>* enemies){
    auto start = std::chrono::system_clock::now().time_since_epoch();
    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start).count();
    if(shootingTime < milliseconds){
        shotFired = true;
        Ray ray(position, aimDir, range, map, enemies);
        shootingTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + shootingRate;
    }
}
