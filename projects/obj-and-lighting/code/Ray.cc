#include "Ray.h"
#include "render/Debug.h"

Ray::Ray(Vector position, Vector direction, int range){
    direction.Normalize();
    Vector endPos = position + (direction * range);
    Debug::DrawLine(position, endPos, Vector(0,0,1));
}

Ray::Ray(Vector position, Vector direction, int range, Map* map){
    direction.Normalize();
    Vector endPos = position + (direction * range);
    Debug::DrawLine(position, endPos, Vector(0,1,0));

    Vector currentPos = position;
    while(range > (position - currentPos).Length()){
        std::shared_ptr<Tile> tile = map->GetTile((int)currentPos.x, (int)currentPos.z);
        tile->debugMode = true;
        if(LookForEnemies(tile)){
            CheckEnemyIntersection(position, direction, tile);
        }
        currentPos = currentPos + direction;
    }
}

bool Ray::LookForEnemies(std::shared_ptr<Tile> tile){
    if(tile->enemiesOnTileIndex.size() > 0){
        return true;
    }else{
        return false;
    }
}

bool Ray::CheckEnemyIntersection(Vector position, Vector direction, std::shared_ptr<Tile> tile){
    return false;
}
