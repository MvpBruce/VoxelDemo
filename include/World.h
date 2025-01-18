#pragma once
#include "Settings.h"

class Chunk;
class World
{
private:
    std::unordered_map<unsigned int, std::shared_ptr<Chunk>> m_mapChunks;

private:
    void BuildChunks();

public:
    World();
    ~World();

    void Render();
    Chunk* GetChunkByIndex(unsigned int nIndex);
    void BuildChunkMesh();
};
