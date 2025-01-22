#pragma once
#include "ToolHandler.h"
#include <glm/glm.hpp>

class World;
class Chunk;
class VoxelHandler : public ToolHandler
{
public:
    VoxelHandler(World* pWorld);
    virtual ~VoxelHandler();

    virtual void Active();
    virtual void Deactive();

    virtual bool MouseLButtonDown();
    virtual bool MouseRbuttonDown();

    virtual bool RayTrace();

private:
    bool GetChunkByPos(glm::ivec3& vWorldPos);
    void RemoveVoxel();

private:
    glm::ivec3 m_vCurrentPos;
    World* m_pWorld;
    int m_nCurVoxelId;
    int m_nCurVoxelIndex;
    Chunk* m_pChunk;
};