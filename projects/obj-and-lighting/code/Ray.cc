#include "Ray.h"
#include "render/Debug.h"
#include <cmath>

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
    for(int i = 0; i < tile->enemiesOnTileIndex.size(); i++){
        Enemy* enemy = &(*enemies)[tile->enemiesOnTileIndex[0]];
        Vector hyp = position - enemy->position;
        Vector hypNormalized = hyp;
        Vector kat = direction;
        hypNormalized.Normalize();
        kat.Normalize();
        float cosAngle = Dot(hypNormalized, kat);
        float angle = acos(cosAngle);
        Vector oKat = hyp * sin(angle);
        if(oKat.Length() < (enemy->position.Length() + enemy->radius)){
            enemy->Kill();
            return true;
        }
    }
    return false;
}
