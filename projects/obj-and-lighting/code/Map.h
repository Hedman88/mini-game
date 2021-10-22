#pragma once
#include "core/mathLib.h"
#include "render/GraphicsNode.h"
#include "entity.h"
#include "enemy.h"
#include <vector>
#include <map>
#include <unordered_map>

class Tile
{
  public:
    Vector pos;
    std::shared_ptr<GraphicsNode> gNode;
    int width, height;
    bool walkable;
    std::map<int, Enemy> enemiesOnTile;

    Tile(int coordX, int coordY, bool walkable);
    void SetGNode(std::shared_ptr<GraphicsNode> gNode);
    int GetNeighbourIndex(int nr);
    void AddEnemyOnTile(Enemy enemy);
};

class Map
{
    //std::vector<std::shared_ptr<Tile>> tiles;
    std::shared_ptr<Tile> tiles[16*16];

  public:
    void GenerateMap();
    void InitTiles(const char* vShaderFile, const char* pShaderFile, const char* textureFile);
    void Draw(Matrix cameraVPMatrix);
    std::shared_ptr<Tile> GetTile(int coordX, int coordY);
};
