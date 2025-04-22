#pragma once
#include "Settings.h"
#include <glm/matrix.hpp>

class VoxelGame;
class World;
class VertexArray;
class VertexBuffer;

struct VertexData
{
    VertexData() = default;
    VertexData(float x, float y, float z, float u, float v, int v_id, int f_id)
        : m_x(x), m_y(y), m_z(z), m_u(u), m_v(v), m_voxel_id(v_id), m_face_id(f_id)
    {}

    float m_x;
    float m_y;
    float m_z;
    float m_u;
    float m_v;
    int m_voxel_id;
    int m_face_id;
};

class Chunk
{
private:
    unsigned int* m_pVoxels;
    VertexData* m_pVertexData;
    unsigned int m_nVertexCount;
    glm::vec3 m_vPosition;
    glm::vec3 m_vCenter;
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
    int GetChunkId(int nIndex);
    void SetVoxelIdByIndex(int nIndex, int nId);
    //Get relative pos in world
    glm::vec3& GetRelPosInWorld();
    
private:
    void BuildVoxels();
    bool IsEmpty(glm::vec3 vLocal, glm::vec3 vWorld);
    void PrepareData();
    void AddVertices(VertexData& v0, VertexData& v1, VertexData& v2);
    int GetIndexInWorld(glm::vec3 vWorld);
    glm::mat4& getModelMatrix();
    VertexData* GetData();
    unsigned int GetSize();
    unsigned int GetCount();
};