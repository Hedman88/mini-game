#include "Ray.h"
#include "render/Debug.h"

Ray::Ray(Vector position, Vector direction, int range){
    direction.Normalize();
    Vector endPos = position + (direction * range);
    Debug::DrawLine(position, endPos, Vector(0,0,1));
}

Ray::Ray(Vector position, Vector direction, int range, Map* map, std::vector<Enemy>* enemies){
    direction.Normalize();
    Vector endPos = position + (direction * range);
    Debug::DrawLine(position, endPos, Vector(0,1,0));
    Vector currentPos = position;
    while(range > (position - currentPos).Length()){
        std::shared_ptr<Tile> tile = map->GetTile((int)currentPos.x, (int)currentPos.z);
        if(!tile->walkable){
            break;
        }
        tile->debugMode = true;
        if(LookForEnemies(tile)){
            CheckEnemyIntersection(position, direction, tile, enemies);
        }
        currentPos = currentPos + direction*0.1f;
    }
}

bool Ray::LookForEnemies(std::shared_ptr<Tile> tile){
    if(tile->enemiesOnTileIndex.size() > 0){
        return true;
    }else{
        return false;
    }
}

bool Ray::CheckEnemyIntersection(Vector position, Vector direction, std::shared_ptr<Tile> tile, std::vector<Enemy>* enemies){
    return false;
}
