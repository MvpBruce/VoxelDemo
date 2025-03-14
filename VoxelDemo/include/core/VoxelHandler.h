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
    void SetOperatorMode(int nType);
    int GetOperatorMode();


private:
    bool GetChunkByPos(glm::ivec3& vWorldPos);
    void RemoveVoxel();
    void AddVoxel();
    void Reset();
    void PrepareData();
    void RebuildAdjacentChunks();
    void RebuildAdjacentChunk(glm::ivec3& vWorldPos);

private:
    glm::ivec3 m_vCurWroldPos;
    glm::ivec3 m_vCurLocalPos;
    World* m_pWorld;
    int m_nCurVoxelId;
    int m_nCurVoxelIndex;
    Chunk* m_pChunk;
    glm::ivec3 m_vNormal;
    int m_nOperatorMode;
    bool m_bLoaded;
    std::shared_ptr<VertexBuffer> m_pVB;
    std::shared_ptr<VertexArray> m_pVA;
    float* m_pVertexData;
    unsigned int m_nVertexCount;
};