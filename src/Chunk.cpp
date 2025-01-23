#include "Chunk.h"
#include <glm/gtc/noise.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VoxelGame.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "World.h"
#include "ShaderManager.h"

Chunk::Chunk(glm::vec3 vPosition, World* pWorld): m_vPosition(vPosition),
m_pWorld(pWorld), m_bLoaded(false), m_nVertaxCount(0)
{
    m_matModel = glm::translate(glm::mat4(1.0f), m_vPosition * (float)CHUNK_SIZE);
    m_pVoxels = new unsigned int[CHUNK_VOL];
    
    //Each cube will use 36 vertices. 6 faces and 6 veriices each face
    m_pVertices = new unsigned int[CHUNK_VOL * sizeof(VertexData) * 36];
    
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

    if (m_pVertices)
    {
        delete[] m_pVertices;
        m_pVertices = nullptr;
    }
}

glm::mat4 &Chunk::getModelMatrix()
{
    return m_matModel;
}

unsigned int *Chunk::GetData()
{
    return m_pVertices;
}

unsigned int Chunk::GetSize()
{
    return m_nVertaxCount * sizeof(VertexData);
}

unsigned int Chunk::GetCount()
{
    return m_nVertaxCount;
}

int Chunk::GetIndexInWorld(glm::vec3 vWorld)
{
    float cx = vWorld.x / (float)CHUNK_SIZE;
    float cy = vWorld.y / (float)CHUNK_SIZE;
    float cz = vWorld.z / (float)CHUNK_SIZE;

    if (cx < 0 || cx >= WORLD_W || cy < 0 || cy >= WORLD_H || cz < 0 || cz >= WORLD_D)
        return -1;

    return (int)(cx + cz * WORLD_D + cy * WORLD_AREA);
}

int Chunk::GetChunkId(unsigned int nIndex)
{
    return m_pVoxels[nIndex];
}

void Chunk::SetVoxelIdByIndex(unsigned int nIndex, unsigned int nId)
{
    m_pVoxels[nIndex] = nId;
}

glm::vec3 &Chunk::GetRelPosInWorld()
{
    return m_vPosition;
}

void Chunk::AddVertices(VertexData &v0, VertexData &v1, VertexData &v2)
{
    if (!m_pVertices)
        return;

    memcpy(m_pVertices + m_nVertaxCount * sizeof(VertexData)/sizeof(unsigned int), &v0, sizeof(VertexData));
    m_nVertaxCount++;

    memcpy(m_pVertices + m_nVertaxCount * sizeof(VertexData)/sizeof(unsigned int), &v1, sizeof(VertexData));
    m_nVertaxCount++;

    memcpy(m_pVertices + m_nVertaxCount * sizeof(VertexData)/sizeof(unsigned int), &v2, sizeof(VertexData));
    m_nVertaxCount++;
}

bool Chunk::IsEmpty(glm::vec3 vLocal, glm::vec3 vWorld)
{
    int nChunkIndex = GetIndexInWorld(vWorld);
    //Out of boundary or not exist
    if (nChunkIndex < 0)
        return true;

    Chunk* pChunk = m_pWorld->GetChunkByIndex(nChunkIndex);
    if (!pChunk)
        return true;

    int nVoxelIndex = (int)vLocal.x % CHUNK_SIZE + (int)vLocal.z % CHUNK_AREA + (int)vLocal.y % CHUNK_VOL;
    //Voxel is not empty
    if (pChunk->GetChunkId(nVoxelIndex) != 0)
        return false;
    
    return true;
}

void Chunk::Render()
{
    PrepareData();
    m_ptrVertexArray->Bind();
    ShaderManager::GetInstance().GetShader("chunk")->SetMatrix("model", m_matModel);  
    CALLERROR(glDrawArrays(GL_TRIANGLES, 0, GetCount()));
}

void Chunk::BuildChunkMesh()
{
    m_bLoaded = false;
    memset(m_pVertices, 0, CHUNK_VOL * sizeof(VertexData) * 36 * sizeof(unsigned int));
    m_nVertaxCount = 0;
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
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

                    AddVertices(v0, v3, v2);
                    AddVertices(v0, v2, v1);
                }

                //bottom face
                if (IsEmpty(glm::vec3(x, y - 1, z), glm::vec3(vWorldPos.x, vWorldPos.y - 1, vWorldPos.z)))
                {
                    VertexData v0(x,     y, z, nVoxelId, 1);
                    VertexData v1(x + 1, y, z, nVoxelId, 1);
                    VertexData v2(x + 1, y, z + 1, nVoxelId, 1);
                    VertexData v3(x,     y, z + 1, nVoxelId, 1);

                    AddVertices(v3, v0, v1);
                    AddVertices(v3, v1, v2);
                }
                
                //right face
                if (IsEmpty(glm::vec3(x + 1, y, z), glm::vec3(vWorldPos.x + 1, vWorldPos.y, vWorldPos.z)))
                {
                    VertexData v0(x + 1, y,     z, nVoxelId, 2);
                    VertexData v1(x + 1, y + 1, z, nVoxelId, 2);
                    VertexData v2(x + 1, y + 1, z + 1, nVoxelId, 2);
                    VertexData v3(x + 1, y,     z + 1, nVoxelId, 2);

                    AddVertices(v2, v3, v0);
                    AddVertices(v2, v0, v1);
                }

                //left face
                if (IsEmpty(glm::vec3(x - 1, y, z), glm::vec3(vWorldPos.x - 1, vWorldPos.y, vWorldPos.z)))
                {
                    VertexData v0(x, y,     z,  nVoxelId, 3);
                    VertexData v1(x, y + 1, z,    nVoxelId, 3);
                    VertexData v2(x, y + 1, z + 1, nVoxelId, 3);
                    VertexData v3(x, y,     z + 1, nVoxelId, 3);

                    AddVertices(v1, v0, v3);
                    AddVertices(v1, v3, v2);
                }

                //back face
                if (IsEmpty(glm::vec3(x, y, z - 1), glm::vec3(vWorldPos.x, vWorldPos.y, vWorldPos.z - 1)))
                {
                    VertexData v0(x,     y,     z, nVoxelId, 4);
                    VertexData v1(x,     y + 1, z, nVoxelId, 4);
                    VertexData v2(x + 1, y + 1, z, nVoxelId, 4);
                    VertexData v3(x + 1, y,     z, nVoxelId, 4);

                    AddVertices(v2, v3, v0);
                    AddVertices(v2, v0, v1);
                }

                //front face
                if (IsEmpty(glm::vec3(x, y, z + 1), glm::vec3(vWorldPos.x, vWorldPos.y, vWorldPos.z + 1)))
                {
                    VertexData v0(x,    y,      z + 1, nVoxelId, 5);
                    VertexData v1(x,    y + 1,  z + 1, nVoxelId, 5);
                    VertexData v2(x + 1, y + 1, z + 1, nVoxelId, 5);
                    VertexData v3(x + 1, y,     z + 1, nVoxelId, 5);

                    AddVertices(v1, v0, v3);
                    AddVertices(v1, v3, v2);
                }
            }
        }
    }
}

void Chunk::BuildVoxels()
{
    memset(m_pVoxels, 0, CHUNK_VOL * sizeof(unsigned int));
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
            //for (int y = 0; y < CHUNK_SIZE; y++)
            {
                wy = y + vPos.y;
                //m_pVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = 1;
                m_pVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = 1 + wy;
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
