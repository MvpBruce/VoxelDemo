#pragma once
#include "Settings.h"
#include "Chunk.hpp"

class World
{
private:
    std::vector<Chunk> m_vChunks;
public:
    World(/* args */)
    {

    }
    ~World() {}

    void BuildChunks()
    {
        for (int x = 0; x < WORLD_W; x++)
        {
            for (int y = 0; y < WORLD_H; y++)
            {
                for (int z = 0; z < WORLD_D; z++)
                {
                    /* code */
                }
                
            }
            
        }
        
    }


};
