#pragma once
#include "Settings.h"
#include <glm/matrix.hpp>

class VoxelGame;
class World;
class VertexArray;
class VertexBuffer;
class Chunk
{
private:
    struct VertexData
    {
        VertexData() = default;
        VertexData(unsigned int x, unsigned int y, unsigned int z, unsigned int v_id, unsigned int f_id)
        : m_x(x), m_y(y), m_z(z), m_voxel_id(v_id), m_face_id(f_id) {}
    
        unsigned int m_x;
        unsigned int m_y;
        unsigned int m_z;
        unsigned int m_voxel_id;
        unsigned int m_face_id;
    };

    unsigned int* m_pVoxels;
    unsigned int* m_pVertices;
    unsigned int m_nVertaxCount;
    glm::vec3 m_vPosition;
    glm::mat4 m_matModel;
    World* m_pWorld;
    bool m_bLoaded;
    std::shared_ptr<VertexArray> m_ptrVertexArray;
    std::shared_ptr<VertexBuffer> m_ptrVertexBuffer;
    
public:
    Chunk(glm::vec3 vPosition, World* pWorld);
    ~Chunk();
 
    void Render();
    void BuildChunkMesh();
    int GetChunkId(unsigned int nIndex);
    void SetVoxelIdByIndex(unsigned int nIndex, unsigned int nId);
    
private:
    void BuildVoxels();
    bool IsEmpty(glm::vec3 vLocal, glm::vec3 vWorld);
    void PrepareData();
    void AddVertices(VertexData& v0, VertexData& v1, VertexData& v2);
    int GetIndexInWorld(glm::vec3 vWorld);
    glm::mat4& getModelMatrix();
    unsigned int* GetData();
    unsigned int GetSize();
    unsigned int GetCount();
};