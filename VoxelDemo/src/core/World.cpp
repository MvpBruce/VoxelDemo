#include "World.h"
#include "Chunk.h"
#include <iostream>
#include <time.h>

World::World()
{
    srand((unsigned long)time(NULL));
    m_nSeed = (int)(((float)rand() / (float)RAND_MAX) * 1000.0f);
    BuildChunks();
}

World::~World()
{
}

void World::BuildChunks()
{
    for (int x = 0; x < WORLD_W; x++)
    {
        for (int y = 0; y < WORLD_H; y++)
        {
            for (int z = 0; z < WORLD_D; z++)
            {
                unsigned int nWorldChunkIndex = x + WORLD_D * z + WORLD_AREA * y;
                //std::cout << "Chunk index: " << nWorldChunkIndex << std::endl;
                m_mapChunks.emplace(nWorldChunkIndex, std::make_shared<Chunk>(glm::vec3(x, y, z), this));
            }
        }
    }
}

void World::Render()
{
    for (auto& it : m_mapChunks)
    {
        it.second->Render();
    }
}

Chunk *World::GetChunkByIndex(unsigned int nIndex)
{
    auto it = m_mapChunks.find(nIndex);
    if (it == m_mapChunks.end())
        return nullptr;
    
    return it->second.get();
}

void World::BuildChunkMesh()
{
    for (auto& it : m_mapChunks)
    {
        it.second->BuildChunkMesh();
    }

    std::cout << "Finished buildding chunk meshes" << std::endl;
}

int World::GetWorldSeed()
{
    return m_nSeed;
}
