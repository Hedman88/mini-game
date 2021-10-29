#include "config.h"
#include "player.h"
#include "Map.h"
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

void Player::Shoot(Map* map){
    auto start = std::chrono::system_clock::now().time_since_epoch();
    long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start).count();
    if(shootingTime < milliseconds){
        Ray ray(position, aimDir, range, map);
        shootingTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + shootingRate;
    }
}
