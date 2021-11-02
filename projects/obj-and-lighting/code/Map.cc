#include "Map.h"
#include <cstdlib>
#include <ctime>
#include "render/Debug.h"

Tile::Tile(int coordX, int coordY, bool walkable, int ID){
    this->pos.x = coordX;
    this->pos.y = 0;
    this->pos.z = coordY;
    this->pos.w = 1;
    this->width = 1;
    this->height = 1;
    this->walkable = walkable;
    this->ID = ID;
}

void Tile::AddEnemy(int enemyIndex){
    this->enemiesOnTileIndex.push_back(enemyIndex);
}

int Tile::RemoveEnemy(int enemyIndex){
    for(int i = 0; i < this->enemiesOnTileIndex.size(); i++){
        if(this->enemiesOnTileIndex[i] == enemyIndex){
            if(enemiesOnTileIndex.end() == this->enemiesOnTileIndex.erase(this->enemiesOnTileIndex.begin() + i)){
                //std::cout << "ALL ELEMENTS REMOVED" << std::endl;
            }
            return 0;
        }
    }
    return -1;
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
                std::shared_ptr<Tile> tileP = std::make_shared<Tile>(x, z, false, z*x);
                this->tiles[z*16+x] = tileP;
            }
            else if(r > wallShare)
            {
                std::shared_ptr<Tile> tileP = std::make_shared<Tile>(x, z, true, z*x);
                this->tiles[z*16+x] = tileP;
            }else{
                std::shared_ptr<Tile> tileP = std::make_shared<Tile>(x, z, false, z*x);
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
        if(debugMode){
            Debug::DrawSquare(0.8, Vector(tiles[i]->pos.x + 0.5f, 0.1, tiles[i]->pos.z + 0.5f), Vector(1,0,0));
            if(tiles[i]->enemiesOnTileIndex.size() > 0){
                Debug::DrawSquare(0.6, Vector(tiles[i]->pos.x + 0.5f, 0.3, tiles[i]->pos.z + 0.5f), Vector(0,1,1));
            }
            if(tiles[i]->debugMode){
                Debug::DrawSquare(0.7, Vector(tiles[i]->pos.x + 0.5f, 0.2, tiles[i]->pos.z + 0.5f), Vector(0,0,1));
                tiles[i]->debugMode = false;
            }
        }
    }
}

std::shared_ptr<Tile> Map::GetTile(int coordX, int coordZ){
    return this->tiles[coordX + 16 * coordZ];
}
