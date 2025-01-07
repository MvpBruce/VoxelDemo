#pragma once
#include "Settings.h"

class Chunk;
class VoxelGame;
class World
{
private:
    std::unordered_map<unsigned int, std::shared_ptr<Chunk>> m_mapChunks;
    VoxelGame* m_pGame;
private:
    void BuildChunks();

public:
    World(VoxelGame* pGame);
    ~World();

    void Render();
};
