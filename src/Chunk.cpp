#include "Chunk.h"
#include <glm/gtc/noise.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VoxelGame.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"

Chunk::Chunk(VoxelGame* pGame, glm::vec3 vPosition): m_pGame(pGame), m_vPosition(vPosition)
{
    m_matModel = glm::translate(glm::mat4(1.0f), m_vPosition * (float)CHUNK_SIZE);
    BuildVoxels();
    BuildChunkMesh();
    PrepareData();
}

Chunk::~Chunk()
{
}

glm::mat4 &Chunk::getModelMatrix()
{
    return m_matModel;
}

unsigned int *Chunk::GetData()
{
    return (unsigned int*)(m_vVertices.data());
}

unsigned int Chunk::GetSize()
{
    return m_vVertices.size() * sizeof(VertexData);
}

unsigned int Chunk::GetCount()
{
    return m_vVertices.size();
}

bool Chunk::IsEmpty(int x, int y, int z, std::vector<unsigned int> &vVoxels)
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
        return true;

    if (vVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] == 1)
        return false;

    return true;
}

void Chunk::Render()
{
    //m_ptrVertexArray->Bind();
    //m_pGame->m_ptrVertexArray->Bind();
    Shader::GetInstance().SetMatrix("model", m_matModel);  
    CALLERROR(glDrawArrays(GL_TRIANGLES, 0, GetCount()));
}

void Chunk::BuildChunkMesh()
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

void Chunk::BuildVoxels()
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

void Chunk::PrepareData()
{
    // if (!m_ptrVertexBuffer)
    //     m_ptrVertexBuffer = std::make_shared<VertexBuffer>(GetData(), GetSize());

    // VertexBufferLayout vLayout;
    // vLayout.AddLayout<unsigned int>(3);
    // vLayout.AddLayout<unsigned int>(1);
    // vLayout.AddLayout<unsigned int>(1);

    // if (!m_ptrVertexArray)
    //     m_ptrVertexArray = std::make_shared<VertexArray>(vLayout);

    m_pGame->m_ptrVertexBuffer->AttachData(GetData(), GetSize());
    VertexBufferLayout vLayout;
    vLayout.AddLayout<unsigned int>(3);
    vLayout.AddLayout<unsigned int>(1);
    vLayout.AddLayout<unsigned int>(1);

    m_pGame->m_ptrVertexArray->AddLayout(vLayout);
}
