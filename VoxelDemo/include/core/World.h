#pragma once
#include "Settings.h"

class Chunk;
class World
{
private:
    std::unordered_map<unsigned int, std::shared_ptr<Chunk>> m_mapChunks;
    int m_nSeed;

private:
    void BuildChunks();

public:
    World();
    ~World();

    void Render();
    Chunk* GetChunkByIndex(unsigned int nIndex);
    void BuildChunkMesh();
    int GetWorldSeed();
};
