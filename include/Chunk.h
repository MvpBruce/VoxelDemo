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
    std::vector<VertexData> m_vVertices;
    glm::vec3 m_vPosition;
    glm::mat4 m_matModel;
    //VoxelGame* m_pGame;
    World* m_pWorld;
    bool m_bLoaded;
    std::shared_ptr<VertexArray> m_ptrVertexArray;
    std::shared_ptr<VertexBuffer> m_ptrVertexBuffer;
    
public:
    Chunk(/*VoxelGame* pGame, */glm::vec3 vPosition, World* pWorld);
    ~Chunk();
 
    glm::mat4& getModelMatrix();
    unsigned int* GetData();
    unsigned int GetSize();
    unsigned int GetCount();
    bool IsEmpty(glm::vec3 vLocal, glm::vec3 vWorld);
    void Render();
    void BuildChunkMesh();
    void BuildVoxels();
    void PrepareData();
    int GetIndexInWorld(glm::vec3 vWorld);
    int GetChunkValue(unsigned int nIndex);
};