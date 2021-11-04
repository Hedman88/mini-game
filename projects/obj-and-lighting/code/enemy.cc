#include "enemy.h"
#include "Map.h"
#include "Score.h"
#include <ctime>
#include <cmath>

int deadAmount = 0;
int GetDeadAmount(){
    return deadAmount;
}
void SetDeadAmount(int amount){
    deadAmount = amount;
}

Enemy::Enemy(Vector position)
{
    this->position = position;
    //this->graphicNode = std::make_shared<GraphicsNode>();
}

Enemy::~Enemy()
{
    //if (score != nullptr)
    //{
    //    score->IncrementScore();
    //}
    //// Remove this enemy from all previously occupied tiles
    //for(int i = 0; i < currentTiles.size(); i++){
    //    currentTiles[i]->RemoveEnemy(ID);
    //}
    //currentTiles.clear();
}

// spawn waves onto map
void Enemy::SpawnEnemies(std::vector<Enemy>* enemies, Map* map, unsigned int waves, Player* player, const unsigned int width, const unsigned int height)
{
    if (enemies == nullptr)
    {
        return;
    }
    if (player == nullptr)
    {
        return;
    }
    enemies->clear();
    if (enemies->size() > 0)
    {
        return;
    }

    srand((unsigned int)time(NULL));

    for (size_t i = 0; i < 0 + (int)(waves * 6.09f); i++)
    {
        int x, z;
        int the_tangent_of_the_cameras_fov_divided_by_the_height_given_as_a_constant_in_the_cameras_projection_matrix = 5;
        do
        {
            x = rand() % width, z = rand() % height;
            if (map->GetTile(x, z)->walkable &&
            abs(x - player->position.x) > the_tangent_of_the_cameras_fov_divided_by_the_height_given_as_a_constant_in_the_cameras_projection_matrix &&
            abs(z - player->position.z) > the_tangent_of_the_cameras_fov_divided_by_the_height_given_as_a_constant_in_the_cameras_projection_matrix) break;
        }while(true);
        Vector position = Vector(x, 0, z);
        Enemy en(position);
        enemies->push_back(en);
    }
}

void Enemy::Update(Player player, Map* map){
    if(dead){
        return;
    }
    // Movement
    if (player.position.x - position.x > 0.f && // player is right of the enemy, so the enemies attempts to move right
    map->GetTile(int(position.x + radius / 2), int(position.z))->walkable)
        UpdateX(player);
    
    if (player.position.x - position.x < 0.f && // player is left of the enemy, so the enemies attempts to move left
    map->GetTile(int(position.x - radius), int(position.z))->walkable)
        UpdateX(player);

    if (player.position.z - position.z > 0.f && // player is under of the enemy, so the enemies must move up
    map->GetTile(int(position.x), int(position.z + radius / 2))->walkable)
        UpdateZ(player);
    
    if (player.position.z - position.z < 0.f && // player is over of the enemy, so the enemies must move down
    map->GetTile(int(position.x), int(position.z - radius))->walkable)
        UpdateZ(player);

    UpdateTiles(map);
}

void Enemy::Kill(){
    dead = true;
    if (score != nullptr)
    {
        score->IncrementScore();
    }
    // Remove this enemy from all previously occupied tiles
    for(int i = 0; i < currentTiles.size(); i++){
        currentTiles[i]->RemoveEnemy(ID);
    }
    currentTiles.clear();
    deadAmount++;
}

void Enemy::UpdateX(Player player)
{
    Vector temp;
    temp = player.position - this->position;
    if (temp.Length())
        temp.Normalize();
    position.x = position.x + temp.x * this->moveSpeed;
}

void Enemy::UpdateZ(Player player)
{
    Vector temp;
    temp = player.position - this->position;
    if (temp.Length())
        temp.Normalize();
    position.z = position.z + temp.z * this->moveSpeed;
}

void Enemy::UpdateTiles(Map* map){
    float x = position.x;
    float z = position.z;
    int coordUp = z - radius;
    int coordLeft = x - radius;
    int coordDown = z + radius;
    int coordRight = x + radius;
    std::shared_ptr<Tile> currentTile = map->GetTile((int)x, (int)z);
    std::shared_ptr<Tile> tUL = map->GetTile(coordLeft, coordUp);
    std::shared_ptr<Tile> tUR = map->GetTile(coordRight, coordUp);
    std::shared_ptr<Tile> tDL = map->GetTile(coordLeft, coordDown);
    std::shared_ptr<Tile> tDR = map->GetTile(coordRight, coordDown);

    // Remove this enemy from all previously occupied tiles
    for(int i = 0; i < currentTiles.size(); i++){
        currentTiles[i]->RemoveEnemy(ID);
    }
    currentTiles.clear();

    // Adding this enemy to all tiles covered by it
    // As well as adding all the tiles to the enemy
    // Starting with diagonal tiles
    currentTile->AddEnemy(ID);
    currentTiles.push_back(currentTile);
    // Up Left
    if(tUL != currentTile && CheckIfTilesContain(tUL) == false){
        tUL->AddEnemy(ID);
        currentTiles.push_back(tUL);

        std::shared_ptr<Tile> left = map->GetTile(coordLeft, z);
        left->AddEnemy(ID);
        currentTiles.push_back(left);

        std::shared_ptr<Tile> up = map->GetTile(x, coordUp);
        up->AddEnemy(ID);
        currentTiles.push_back(up);
        return;
    }
    // Up Right
    if(tUR != currentTile && CheckIfTilesContain(tUR) == false){
        tUR->AddEnemy(ID);
        currentTiles.push_back(tUR);

        std::shared_ptr<Tile> right = map->GetTile(coordRight, z);
        right->AddEnemy(ID);
        currentTiles.push_back(right);

        std::shared_ptr<Tile> up = map->GetTile(x, coordUp);
        up->AddEnemy(ID);
        currentTiles.push_back(up);
        return;
    }
    // Down Left
    if(tDL != currentTile && CheckIfTilesContain(tDR) == false){
        tDL->AddEnemy(ID);
        currentTiles.push_back(tDL);

        std::shared_ptr<Tile> left = map->GetTile(coordLeft, z);
        left->AddEnemy(ID);
        currentTiles.push_back(left);

        std::shared_ptr<Tile> down = map->GetTile(x, coordDown);
        down->AddEnemy(ID);
        currentTiles.push_back(down);
        return;
    }
    // Down Right
    if(tDR != currentTile && CheckIfTilesContain(tDR) == false){
        tDR->AddEnemy(ID);
        currentTiles.push_back(tDR);

        std::shared_ptr<Tile> right = map->GetTile(coordRight, z);
        right->AddEnemy(ID);
        currentTiles.push_back(right);

        std::shared_ptr<Tile> down = map->GetTile(x, coordDown);
        down->AddEnemy(ID);
        currentTiles.push_back(down);
        return;
    }

    // If no diagonal tiles are covered, check just the sides
    std::shared_ptr<Tile> tU = map->GetTile(x, coordUp);
    std::shared_ptr<Tile> tL = map->GetTile(coordLeft, z);
    std::shared_ptr<Tile> tD = map->GetTile(x, coordDown);
    std::shared_ptr<Tile> tR = map->GetTile(coordRight, z);
    // Up
    if(tU != currentTile && CheckIfTilesContain(tU) == false){
        tU->AddEnemy(ID);
        currentTiles.push_back(tU);
    }
    // Left
    if(tL != currentTile && CheckIfTilesContain(tL) == false){
        tL->AddEnemy(ID);
        currentTiles.push_back(tL);
    }
    // Down
    if(tD != currentTile && CheckIfTilesContain(tD) == false){
        tD->AddEnemy(ID);
        currentTiles.push_back(tD);
    }
    // Right
    if(tR != currentTile && CheckIfTilesContain(tR) == false){
        tR->AddEnemy(ID);
        currentTiles.push_back(tR);
    }
}

bool Enemy::CheckIfTilesContain(std::shared_ptr<Tile> tile){
    for(int i = 0; i < currentTiles.size(); i++){
        if(currentTiles[i]->ID == tile->ID){
            std::cout << "TRUE" << std::endl;
            return true;
        }
    }
    return false;
}

void Enemy::SetGNode(std::shared_ptr<GraphicsNode> gNode){
    this->graphicNode = gNode;
}
