#include "Chunk.h"
#include <glm/gtc/noise.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VoxelGame.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "World.h"

Chunk::Chunk(glm::vec3 vPosition, World* pWorld): m_vPosition(vPosition),
m_pWorld(pWorld), m_bLoaded(false)
{
    m_matModel = glm::translate(glm::mat4(1.0f), m_vPosition * (float)CHUNK_SIZE);
    m_pVoxels = new unsigned int[CHUNK_VOL];
    memset(m_pVoxels, 0, CHUNK_VOL * sizeof(unsigned int));
    m_ptrVertexArray = std::make_shared<VertexArray>();
    m_ptrVertexBuffer = std::make_shared<VertexBuffer>();
    BuildVoxels();
}

Chunk::~Chunk()
{
    if (m_pVoxels)
    {
        delete[] m_pVoxels;
        m_pVoxels = nullptr;
    } 
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

int Chunk::GetIndexInWorld(glm::vec3 vWorld)
{
    int cx = vWorld.x / CHUNK_SIZE;
    int cy = vWorld.y / CHUNK_SIZE;
    int cz = vWorld.z / CHUNK_SIZE;

    if (cx < 0 || cx >= WORLD_W || cy < 0 || cy >= WORLD_H || cz < 0 || cz >= WORLD_D)
        return -1;

    return (int)(cx + cz * WORLD_D + cy * WORLD_AREA);
}

int Chunk::GetChunkValue(unsigned int nIndex)
{
    int nValue = m_pVoxels[nIndex];
    return nValue;
}

bool Chunk::IsEmpty(glm::vec3 vLocal, glm::vec3 vWorld)
{
    int nChunkIndex = GetIndexInWorld(vWorld);
    //Out of boundary or not exist
    if (nChunkIndex < 0)
        return false;

    Chunk* pChunk = m_pWorld->GetChunkByIndex(nChunkIndex);
    int nVoxelIndex = (int)vLocal.x % CHUNK_SIZE + (int)vLocal.z % CHUNK_SIZE * CHUNK_SIZE + (int)vLocal.y % CHUNK_SIZE * CHUNK_AREA;
    //Voxel is not empty
    if (pChunk->GetChunkValue(nVoxelIndex) != 0)
        return false;
    
    return true;
}

void Chunk::Render()
{
    PrepareData();
    m_ptrVertexArray->Bind();
    Shader::GetInstance().SetMatrix("model", m_matModel);  
    CALLERROR(glDrawArrays(GL_TRIANGLES, 0, GetCount()));
    //std::cout << "Count: " << GetCount() << std::endl;
}

void Chunk::BuildChunkMesh()
{
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                unsigned int nVoxelId = m_pVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
                //0 is empty
                if (nVoxelId == 0)
                    continue;

                glm::vec3 vWorldPos = m_vPosition * (float)CHUNK_SIZE;
                vWorldPos.x += x;
                vWorldPos.y += y;
                vWorldPos.z += z;
                
                //top face
                if (IsEmpty(glm::vec3(x, y + 1, z), glm::vec3(vWorldPos.x, vWorldPos.y + 1, vWorldPos.z)))
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
                if (IsEmpty(glm::vec3(x, y - 1, z), glm::vec3(vWorldPos.x, vWorldPos.y - 1, vWorldPos.z)))
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
                if (IsEmpty(glm::vec3(x + 1, y, z), glm::vec3(vWorldPos.x + 1, vWorldPos.y, vWorldPos.z)))
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
                if (IsEmpty(glm::vec3(x - 1, y, z), glm::vec3(vWorldPos.x - 1, vWorldPos.y, vWorldPos.z)))
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
                if (IsEmpty(glm::vec3(x, y, z - 1), glm::vec3(vWorldPos.x, vWorldPos.y, vWorldPos.z - 1)))
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
                if (IsEmpty(glm::vec3(x, y, z + 1), glm::vec3(vWorldPos.x, vWorldPos.y, vWorldPos.z + 1)))
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
    glm::ivec3 vPos = glm::ivec3(m_vPosition) * (int)CHUNK_SIZE;
    int wx = 0, wy = 0, wz = 0, nWorldHeight = 0, nLocalHeight = 0;
    for (int x = 0; x < CHUNK_SIZE; x++)
    {   
        wx = x + vPos.x;
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            wz = z + vPos.z;
            nWorldHeight = glm::simplex(glm::vec2(wx, wz) * 0.01f) * 32 + 32;
            nLocalHeight = glm::min(nWorldHeight - vPos.y, (int)CHUNK_SIZE);
            for (int y = 0; y < nLocalHeight; y++)
            {
                wy = y + vPos.y;
                m_pVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = wy + 1;
                //std::cout << wy + 1 << std::endl;
            }
        }
    }
}

void Chunk::PrepareData()
{
    if (m_bLoaded)
        return;

    m_ptrVertexBuffer->AttachData(GetData(), GetSize());
    VertexBufferLayout vLayout;
    vLayout.AddLayout<unsigned int>(3);
    vLayout.AddLayout<unsigned int>(1);
    vLayout.AddLayout<unsigned int>(1);

    m_ptrVertexArray->AddLayout(vLayout);
    m_bLoaded = true;
}
