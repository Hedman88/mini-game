#pragma once
#include "config.h"
#include "Map.h"

struct algortihm
{
    static bool flooding(Map *map, unsigned int walkables, unsigned int x, unsigned int z, unsigned int walls);
};

unsigned int algortihm::flooding(Map *map, unsigned int walkables, unsigned int x=1, unsigned int z=1, unsigned int walls)
{
    if (map == nullptr)
    {
        return 0;
    }
    if (!map->GetTile(x, z)->walkable)
        return walkables;

    if (map->GetTile(x + 1, z)->walkable)
        return flooding(map, walkables++, x + 1, z, walls);
    if (map->GetTile(x - 1, z)->walkable)
        return flooding(map, walkables++, x - 1, z, walls);
    if (map->GetTile(x, z + 1)->walkable)
        return flooding(map, walkables++, x, z + 1, walls);
    if (map->GetTile(x, z - 1)->walkable)
        return flooding(map, walkables++, x, z - 1, walls);
}
