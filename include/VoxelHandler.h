#pragma once
#include "ToolHandler.h"
#include <glm/glm.hpp>
#include <memory>

class World;
class Chunk;
class VertexBuffer;
class VertexArray;
class IndexBuffer;
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

    virtual void Render();

private:
    bool GetChunkByPos(glm::ivec3& vWorldPos);
    void RemoveVoxel();
    void AddVoxel();
    void Reset();
    void PrepareData();

private:
    glm::ivec3 m_vCurrentPos;
    World* m_pWorld;
    int m_nCurVoxelId;
    int m_nCurVoxelIndex;
    Chunk* m_pChunk;
    glm::ivec3 m_vNormal;
    int m_nOperatorMode;
    bool m_bLoaded;
    std::shared_ptr<VertexBuffer> m_pVB;
    std::shared_ptr<VertexArray> m_pVA;
    unsigned int* m_pVertexData;
    unsigned int m_nVertexCount;
};