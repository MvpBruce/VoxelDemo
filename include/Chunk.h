#pragma once
#include "Settings.h"
#include <glm/matrix.hpp>

// class VertexArray;
// class VertexBuffer;
class VoxelGame;
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

    std::vector<unsigned int> m_vVoxels;
    std::vector<VertexData> m_vVertices;
    glm::vec3 m_vPosition;
    glm::mat4 m_matModel;
    VoxelGame* m_pGame;
    //std::shared_ptr<VertexArray> m_ptrVertexArray;
    //std::shared_ptr<VertexBuffer> m_ptrVertexBuffer;
    
public:
    Chunk(VoxelGame* pGame, glm::vec3 vPosition);
    ~Chunk();
 
    glm::mat4& getModelMatrix();
    unsigned int* GetData();
    unsigned int GetSize();
    unsigned int GetCount();
    bool IsEmpty(int x, int y, int z, std::vector<unsigned int>& vVoxels);
    void Render();
    void BuildChunkMesh();
    void BuildVoxels();
    void PrepareData();
};