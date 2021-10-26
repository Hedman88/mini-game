#include "Map.h"
#include <cstdlib>
#include <ctime>

Tile::Tile(int coordX, int coordY, bool walkable){
    this->pos.x = coordX;
    this->pos.y = 0;
    this->pos.z = coordY;
    this->pos.w = 1;
    this->width = 1;
    this->height = 1;
    this->walkable = walkable;
}

void Tile::AddEnemy(Enemy enemy){
    this->enemiesOnTile.push_back(enemy);
}

void Tile::SetGNode(std::shared_ptr<GraphicsNode> gNode){
    this->gNode = gNode;
}


//------------------------------MAP-------------------------------------//
//----------------------------------------------------------------------//
void Map::GenerateMap(int maxRand, int wallShare){

    srand((int)std::time(0));
    int r;
    for(int z = 0; z < 16; z++){
        for(int x = 0; x < 16; x++){
            if(z == 0 || x == 0 || z == 15 || x == 15){
                std::shared_ptr<Tile> tileP = std::make_shared<Tile>(x, z, false);
                this->tiles[z*16+x] = tileP;
            }
            else if(r > wallShare)
            {
                std::shared_ptr<Tile> tileP = std::make_shared<Tile>(x, z, true);
                this->tiles[z*16+x] = tileP;
            }else{
                std::shared_ptr<Tile> tileP = std::make_shared<Tile>(x, z, false);
                this->tiles[z*16+x] = tileP;
            }
            r = (rand() % maxRand) + 1;
        }
    }
}

void Map::InitTiles(const char* vShaderFile, const char* pShaderFile, const char* roadTexture, const char* wallTexture){
    std::shared_ptr<GraphicsNode> gNodeWalkable = std::make_shared<GraphicsNode>("assets/kenney_retroUrbanKit/Models/OBJ_format/grass.obj");
    gNodeWalkable->InitNode(vShaderFile, pShaderFile, roadTexture);
    std::shared_ptr<GraphicsNode> gNodeWall = std::make_shared<GraphicsNode>("assets/kenney_retroUrbanKit/Models/OBJ_format/wallA_roof.obj");
    gNodeWall->InitNode(vShaderFile, pShaderFile, wallTexture);

    for(int i = 0; i < 16*16; i++){
        if(this->tiles[i]->walkable){
            this->tiles[i]->SetGNode(gNodeWalkable);
        }else if(!this->tiles[i]->walkable){
            this->tiles[i]->SetGNode(gNodeWall);
        }
        //if(i != 0) this->tiles[i]->gNode->SetSR(this->tiles[0]->gNode->GetSR());
    }
}

void Map::Draw(Matrix cameraVPMatrix){
    for(int i = 0; i < 16*16; i++){
        this->tiles[i]->gNode->Draw(cameraVPMatrix, PositionMat(tiles[i]->pos + Vector(0.5f, 0, 0.5f)));
    }
}

std::shared_ptr<Tile> Map::GetTile(int coordX, int coordZ){
    return this->tiles[coordX + 16 * coordZ];
}
