#pragma once
#include "Settings.h"
#include <glm/gtc/noise.hpp>

class Chunk
{
private:
    struct VertexData
    {
        VertexData() = default;
        VertexData(unsigned int x, unsigned int y, unsigned int z, unsigned int v_id, unsigned int f_id)
        : m_x(x), m_y(y), m_z(z), m_voxel_id(v_id), m_face_id(f_id)
        {

        }
        
        unsigned int m_x;
        unsigned int m_y;
        unsigned int m_z;
        unsigned int m_voxel_id;
        unsigned int m_face_id;
    };

    enum class Face:unsigned int
    {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGH,
        FRONT,
        BACK
    };

    std::vector<unsigned int> m_vVoxels;
    std::vector<VertexData> m_vVertices;
    
public:
    Chunk(/* args */)
    {
        BuildVoxels();
        BuildChunkMesh();
    }

    ~Chunk()
    {
        
    }

    unsigned int* GetData()
    {
        return (unsigned int*)(m_vVertices.data());
    }

    unsigned int GetSize()
    {
        return m_vVertices.size() * sizeof(VertexData);
    }

    unsigned int GetCount()
    {
        return m_vVertices.size();
    }

    bool IsEmpty(int x, int y, int z, std::vector<unsigned int>& vVoxels)
    {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
            return true;

        if (vVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] == 1)
            return false;

        return true;
    }

    void BuildChunkMesh()
    {
        for (unsigned int x = 0; x < CHUNK_SIZE; x++)
        {
            for (unsigned int y = 0; y < CHUNK_SIZE; y++)
            {
                for (unsigned int z = 0; z < CHUNK_SIZE; z++)
                {
                    unsigned int nVoxelId = m_vVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
                    //0 is empty
                    if (nVoxelId == 0)
                        continue;

                    //top face
                    if (IsEmpty(x, y + 1, z, m_vVoxels))
                    {
                        VertexData v0(x,     y + 1, z, nVoxelId, 0);
                        VertexData v1(x + 1, y + 1, z, nVoxelId, 0);
                        VertexData v2(x + 1, y + 1, z + 1, nVoxelId, 0);
                        VertexData v3(x,     y + 1, z + 1, nVoxelId, 0);
                        m_vVertices.push_back(v0);
                        m_vVertices.push_back(v3);
                        m_vVertices.push_back(v2);

                        m_vVertices.push_back(v0);
                        m_vVertices.push_back(v2);
                        m_vVertices.push_back(v1);
                    }

                    //bottom face
                    if (IsEmpty(x, y - 1, z, m_vVoxels))
                    {
                        VertexData v0(x,     y, z, nVoxelId, 1);
                        VertexData v1(x + 1, y, z, nVoxelId, 1);
                        VertexData v2(x + 1, y, z + 1, nVoxelId, 1);
                        VertexData v3(x,     y, z + 1, nVoxelId, 1);
                        m_vVertices.push_back(v3);
                        m_vVertices.push_back(v0);
                        m_vVertices.push_back(v1);

                        m_vVertices.push_back(v3);
                        m_vVertices.push_back(v1);
                        m_vVertices.push_back(v2);
                    }
                    
                    //right face
                    if (IsEmpty(x + 1, y, z, m_vVoxels))
                    {
                        VertexData v0(x + 1, y,     z, nVoxelId, 2);
                        VertexData v1(x + 1, y + 1, z, nVoxelId, 2);
                        VertexData v2(x + 1, y + 1, z + 1, nVoxelId, 2);
                        VertexData v3(x + 1, y,     z + 1, nVoxelId, 2);
                        m_vVertices.push_back(v2);
                        m_vVertices.push_back(v3);
                        m_vVertices.push_back(v0);

                        m_vVertices.push_back(v2);
                        m_vVertices.push_back(v0);
                        m_vVertices.push_back(v1);
                    }

                    //left face
                    if (IsEmpty(x - 1, y, z, m_vVoxels))
                    {
                        VertexData v0(x, y,     z,  nVoxelId, 3);
                        VertexData v1(x, y + 1, z,    nVoxelId, 3);
                        VertexData v2(x, y + 1, z + 1, nVoxelId, 3);
                        VertexData v3(x, y,     z + 1, nVoxelId, 3);
                        m_vVertices.push_back(v1);
                        m_vVertices.push_back(v0);
                        m_vVertices.push_back(v3);

                        m_vVertices.push_back(v1);
                        m_vVertices.push_back(v3);
                        m_vVertices.push_back(v2);
                    }

                    //back face
                    if (IsEmpty(x, y, z - 1, m_vVoxels))
                    {
                        VertexData v0(x,     y,     z, nVoxelId, 4);
                        VertexData v1(x,     y + 1, z, nVoxelId, 4);
                        VertexData v2(x + 1, y + 1, z, nVoxelId, 4);
                        VertexData v3(x + 1, y,     z, nVoxelId, 4);
                        m_vVertices.push_back(v2);
                        m_vVertices.push_back(v3);
                        m_vVertices.push_back(v0);

                        m_vVertices.push_back(v2);
                        m_vVertices.push_back(v0);
                        m_vVertices.push_back(v1);
                    }

                    //front face
                    if (IsEmpty(x, y, z + 1, m_vVoxels))
                    {
                        VertexData v0(x,    y,      z + 1, nVoxelId, 5);
                        VertexData v1(x,    y + 1,  z + 1, nVoxelId, 5);
                        VertexData v2(x + 1, y + 1, z + 1, nVoxelId, 5);
                        VertexData v3(x + 1, y,     z + 1, nVoxelId, 5);
                        m_vVertices.push_back(v1);
                        m_vVertices.push_back(v0);
                        m_vVertices.push_back(v3);

                        m_vVertices.push_back(v1);
                        m_vVertices.push_back(v3);
                        m_vVertices.push_back(v2);
                    }
                }
            }
        }
    }

    void BuildVoxels()
    {
        if (m_vVoxels.empty())
        {
            m_vVoxels.resize(CHUNK_VOL);

            //Init voxels type, type value from 0-255
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    for (int y = 0; y < CHUNK_SIZE; y++)
                    {
                        float fSimplex = glm::simplex(glm::vec3(x, y, z)) + 1;
                        m_vVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = (int)fSimplex;
                    }
                }
            }
        }
    }
};