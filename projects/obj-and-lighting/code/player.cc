#include "config.h"
#include "player.h"
#include "Map.h"
#include "enemy.h"
#include "Ray.h"
#include "GLFW/glfw3.h"
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
    double start = glfwGetTime() * 1000.0f;
    if(shootingTime < start){
        shotFired = true;
        Ray ray(position, aimDir, range, map, enemies);
        shootingTime = start + shootingRate;
    }
}
