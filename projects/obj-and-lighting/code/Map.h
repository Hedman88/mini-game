#pragma once
#include "core/mathLib.h"
#include "render/GraphicsNode.h"
#include "enemy.h"
#include <vector>

class Tile
{
  public:
    Vector pos;
    std::shared_ptr<GraphicsNode> gNode;
    int width, height;
    bool walkable;
    std::vector<int> enemiesOnTileIndex;
    bool debugMode = false;

    Tile(int coordX, int coordY, bool walkable);
    void AddEnemy(int enemyIndex);
    int RemoveEnemy(int enemyIndex);
    void SetGNode(std::shared_ptr<GraphicsNode> gNode);
    int GetNeighbourIndex(int nr);
};

class Map
{
    std::shared_ptr<Tile> tiles[16*16];
    bool debugMode = true;

  public:
    void GenerateMap(int maxRand, int wallShare);
    void InitTiles(const char* vShaderFile, const char* pShaderFile, const char* roadTexture, const char* wallTexture);
    void Draw(Matrix cameraVPMatrix);
    std::shared_ptr<Tile> GetTile(int coordX, int coordY);
};
