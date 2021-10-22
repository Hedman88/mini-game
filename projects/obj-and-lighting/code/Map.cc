#include "Map.h"

Tile::Tile(int coordX, int coordY, bool walkable){
    this->pos.x = coordX;
    this->pos.y = 0;
    this->pos.z = coordY;
    this->pos.w = 1;
    //this->topNeighbourIndex = topIdx;
    //this->leftNeighbourIndex = leftIdx;
    //this->bottomNeighbourIndex = botIdx;
    //this->rightNeighbourIndex = rightIdx;
    this->width = 1;
    this->height = 1;
    this->walkable = walkable;
    if(walkable){
        this->gNode.reset(new GraphicsNode("assets/kenney_retroUrbanKit/Models/OBJ_format/roadDirt_center.obj"));
    }else{
        this->gNode.reset(new GraphicsNode("assets/kenney_retroUrbanKit/Models/OBJ_format/tree_small.obj"));
    }
}

void Tile::SetGNode(std::shared_ptr<GraphicsNode> gNode){
    this->gNode = gNode;
}

//int Tile::GetNeighbourIndex(int nr){
//    switch(nr){
//        case 1:
//            return this->topNeighbourIndex;
//            break;
//        case 2:
//            return this->leftNeighbourIndex;
//            break;
//        case 3:
//            return this->bottomNeighbourIndex;
//            break;
//        case 4:
//            return this->rightNeighbourIndex;
//        default:
//            std::cout << "specify a nr from 1-4 corresponding to WASD in order, check Map.cc" << std::endl;
//    }
//    return -1;
//}

void Tile::AddEnemyOnTile(Enemy enemy){
    //this->enemiesOnTile[enemy.id] = enemy;
}

//------------------------------MAP-------------------------------------//
//----------------------------------------------------------------------//
void Map::GenerateMap(){
    for(int y = 0; y < 16; y++){
        for(int x = 0; x < 16; x++){
            if(y == 0 || x == 0 || y == 15 || x == 15){
                std::shared_ptr<Tile> tileP = std::make_shared<Tile>(x, y, false);
                this->tiles[y*16+x] = tileP;
            }
            else{
                std::shared_ptr<Tile> tileP = std::make_shared<Tile>(x, y, true);
                this->tiles[y*16+x] = tileP;
            }
        }
    }
}

void Map::InitTiles(const char* vShaderFile, const char* pShaderFile, const char* textureFile){
    for(int i = 0; i < 16*16; i++){
        this->tiles[i]->gNode->InitNode(vShaderFile, pShaderFile, textureFile);
        if(i != 0) this->tiles[i]->gNode->SetSR(this->tiles[0]->gNode->GetSR());
    }
}

void Map::Draw(Matrix cameraVPMatrix){
    for(int i = 0; i < 16*16; i++){
        this->tiles[i]->gNode->Draw(cameraVPMatrix, PositionMat(tiles[i]->pos));
    }
}

std::shared_ptr<Tile> Map::GetTile(int coordX, int coordY){
    return this->tiles[coordX + 16 * coordY];
}
