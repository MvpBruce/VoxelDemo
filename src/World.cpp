#include "World.h"
#include "Chunk.h"
#include <iostream>
#include "VoxelGame.h"

World::World(VoxelGame* pGame): m_pGame(pGame)
{
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
                unsigned int nWorldChunkIndex = x + WORLD_W * z + WORLD_AREA * y;
                //std::cout << "Chunk index: " << nWorldChunkIndex << std::endl;
                std::shared_ptr<Chunk> pChunk = std::make_shared<Chunk>(m_pGame, glm::vec3(x, y, z), this);
                m_mapChunks.emplace(nWorldChunkIndex, std::move(pChunk));
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
}
