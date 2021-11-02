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
    int ID = -1;
    std::vector<int> enemiesOnTileIndex;
    bool debugMode = false;

    Tile(int coordX, int coordY, bool walkable, int ID);
    void AddEnemy(int enemyIndex);
    int RemoveEnemy(int enemyIndex);
    void SetGNode(std::shared_ptr<GraphicsNode> gNode);
    int GetNeighbourIndex(int nr);
};

class Map
{
  std::shared_ptr<GraphicsNode> gNodeWalkable;
  std::shared_ptr<GraphicsNode> gNodeWall;
  std::shared_ptr<Tile> tiles[16*16];
  bool debugMode = false;

public:
  void GenerateMap(int maxRand, int wallShare);
  void InitTiles(const char* vShaderFile, const char* pShaderFile, const char* roadTexture, const char* wallTexture);
  void Draw(Matrix cameraVPMatrix);
  std::shared_ptr<Tile> GetTile(int coordX, int coordY);
  void ToggleDebugMode();
};
