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
#include "Camera.h"
#include "simplex/SimplexNoise.h"

Chunk::Chunk(glm::vec3 vPosition, World* pWorld): m_vPosition(vPosition),
m_pWorld(pWorld), m_bLoaded(false), m_nVertexCount(0), 
m_pVoxels(nullptr), m_pVertexData(nullptr)
{
    m_vCenter = (m_vPosition + 0.5f) * (float)CHUNK_SIZE;
    m_matModel = glm::translate(glm::mat4(1.0f), m_vPosition * (float)CHUNK_SIZE);
    
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

    if (m_pVertexData)
    {
        delete[] m_pVertexData;
        m_pVertexData = nullptr;
    }
}

glm::mat4 &Chunk::getModelMatrix()
{
    return m_matModel;
}

VertexData* Chunk::GetData()
{
    return m_pVertexData;
}

unsigned int Chunk::GetSize()
{
    return m_nVertexCount * sizeof(VertexData);
}

unsigned int Chunk::GetCount()
{
    return m_nVertexCount;
}

int Chunk::GetIndexInWorld(glm::vec3 vWorld)
{
    float cx = vWorld.x / (float)CHUNK_SIZE;
    float cy = vWorld.y / (float)CHUNK_SIZE;
    float cz = vWorld.z / (float)CHUNK_SIZE;

    if (cx < 0 || cx >= WORLD_W || cy < 0 || cy >= WORLD_H || cz < 0 || cz >= WORLD_D)
        return -1;

    return ((int)cx + (int)cz * WORLD_D + (int)cy * WORLD_AREA);
}

int Chunk::GetChunkId(int nIndex)
{
    if (nIndex >= CHUNK_VOL)
        std::cout << "[Get]Chunk index: " << nIndex << " is out of boundary!" << std::endl;

    return m_pVoxels[nIndex];
}

void Chunk::SetVoxelIdByIndex(int nIndex, int nId)
{
    if (nIndex >= CHUNK_VOL)
        std::cout << "[Set]Chunk index: " << nIndex << " is out of boundary!" << std::endl;

    m_pVoxels[nIndex] = nId;
}

glm::vec3 &Chunk::GetRelPosInWorld()
{
    return m_vPosition;
}

void Chunk::AddVertices(VertexData &v0, VertexData &v1, VertexData &v2)
{
    if (!m_pVertexData)
        return;

    memcpy(m_pVertexData + m_nVertexCount, &v0, sizeof(VertexData));
    m_nVertexCount++;

    memcpy(m_pVertexData + m_nVertexCount, &v1, sizeof(VertexData));
    m_nVertexCount++;

    memcpy(m_pVertexData + m_nVertexCount, &v2, sizeof(VertexData));
    m_nVertexCount++;
}

bool Chunk::IsEmpty(glm::vec3 vLocal, glm::vec3 vWorld)
{
    int nChunkIndex = GetIndexInWorld(vWorld);
    //Out of boundary or not exist
    if (nChunkIndex == -1)
        return true;

    Chunk* pChunk = m_pWorld->GetChunkByIndex(nChunkIndex);
    if (!pChunk)
        return true;

    int nVoxelIndex = (int)glm::abs(vLocal.x) % CHUNK_SIZE + (int)glm::abs(vLocal.z) % CHUNK_SIZE * CHUNK_SIZE + (int)glm::abs(vLocal.y) % CHUNK_SIZE * CHUNK_AREA;
    if (nVoxelIndex < 0)
        return true;
        
    //Voxel is not empty
    if (pChunk->GetChunkId(nVoxelIndex))
        return false;
    
    return true;
}

void Chunk::Render()
{
    // if (!GetCamera()->IsInFrustum(m_vCenter))
    //     return;
    
    PrepareData();
    m_ptrVertexArray->Bind();
    ShaderManager::GetInstance().GetShader("chunk")->Use();
    ShaderManager::GetInstance().GetShader("chunk")->SetMatrix("model", m_matModel);  
    CALLERROR(glDrawArrays(GL_TRIANGLES, 0, GetCount()));
}

void Chunk::BuildChunkMesh()
{
    m_bLoaded = false;
    if (!m_pVertexData)
    {
        m_pVertexData = new VertexData[CHUNK_VOL * 36];
        memset(m_pVertexData, 0, sizeof(VertexData) * CHUNK_VOL * 36);
    }

    m_nVertexCount = 0;
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

                //std::cout << "nVoxelId: " << nVoxelId << std::endl;
                glm::vec3 vWorldPos = m_vPosition * (float)CHUNK_SIZE;
                vWorldPos.x += x;
                vWorldPos.y += y;
                vWorldPos.z += z;
                
                //top face
                if (IsEmpty(glm::vec3(x, y + 1, z), glm::vec3(vWorldPos.x, vWorldPos.y + 1, vWorldPos.z)))
                {
                    VertexData v0(x,     y + 1, z,      0, 1, nVoxelId, 1);
                    VertexData v1(x + 1, y + 1, z,      1, 1, nVoxelId, 1);
                    VertexData v2(x + 1, y + 1, z + 1,  1, 0, nVoxelId, 1);
                    VertexData v3(x,     y + 1, z + 1,  0, 0, nVoxelId, 1);

                    AddVertices(v0, v3, v2);
                    AddVertices(v2, v1, v0);
                }

                //bottom face
                if (IsEmpty(glm::vec3(x, y - 1, z), glm::vec3(vWorldPos.x, vWorldPos.y - 1, vWorldPos.z)))
                {
                    VertexData v0(x,     y, z,      0, 0, nVoxelId, 0);
                    VertexData v1(x + 1, y, z,      1, 0, nVoxelId, 0);
                    VertexData v2(x + 1, y, z + 1,  1, 1, nVoxelId, 0);
                    VertexData v3(x,     y, z + 1,  0, 1, nVoxelId, 0);

                    AddVertices(v3, v0, v1);
                    AddVertices(v3, v1, v2);
                }
                
                //right face
                if (IsEmpty(glm::vec3(x + 1, y, z), glm::vec3(vWorldPos.x + 1, vWorldPos.y, vWorldPos.z)))
                {
                    VertexData v0(x + 1, y,     z,      1, 0, nVoxelId, 2);
                    VertexData v1(x + 1, y + 1, z,      1, 1, nVoxelId, 2);
                    VertexData v2(x + 1, y + 1, z + 1,  0, 1, nVoxelId, 2);
                    VertexData v3(x + 1, y,     z + 1,  0, 0, nVoxelId, 2);

                    AddVertices(v2, v3, v0);
                    AddVertices(v2, v0, v1);
                }

                //left face
                if (IsEmpty(glm::vec3(x - 1, y, z), glm::vec3(vWorldPos.x - 1, vWorldPos.y, vWorldPos.z)))
                {
                    VertexData v0(x, y,     z,      0, 0, nVoxelId, 3);
                    VertexData v1(x, y + 1, z,      0, 1, nVoxelId, 3);
                    VertexData v2(x, y + 1, z + 1,  1, 1, nVoxelId, 3);
                    VertexData v3(x, y,     z + 1,  1, 0, nVoxelId, 3);

                    AddVertices(v1, v0, v3);
                    AddVertices(v1, v3, v2);
                }

                //back face
                if (IsEmpty(glm::vec3(x, y, z - 1), glm::vec3(vWorldPos.x, vWorldPos.y, vWorldPos.z - 1)))
                {
                    VertexData v0(x,     y,     z, 1, 0, nVoxelId, 4);
                    VertexData v1(x,     y + 1, z, 1, 1, nVoxelId, 4);
                    VertexData v2(x + 1, y + 1, z, 0, 1, nVoxelId, 4);
                    VertexData v3(x + 1, y,     z, 0, 0, nVoxelId, 4);

                    AddVertices(v2, v3, v0);
                    AddVertices(v2, v0, v1);
                }

                //front face
                if (IsEmpty(glm::vec3(x, y, z + 1), glm::vec3(vWorldPos.x, vWorldPos.y, vWorldPos.z + 1)))
                {
                    VertexData v0(x,    y,      z + 1, 0, 0, nVoxelId, 5);
                    VertexData v1(x,    y + 1,  z + 1, 0, 1, nVoxelId, 5);
                    VertexData v2(x + 1, y + 1, z + 1, 1, 1, nVoxelId, 5);
                    VertexData v3(x + 1, y,     z + 1, 1, 0, nVoxelId, 5);

                    AddVertices(v1, v0, v3);
                    AddVertices(v1, v3, v2);
                }
            }
        }
    }
}

void Chunk::BuildVoxels()
{
    if (!m_pVoxels)
    {
        m_pVoxels = new unsigned int[CHUNK_VOL];
        memset(m_pVoxels, 0, CHUNK_VOL * sizeof(unsigned int));
    }
    
    glm::ivec3 vChunkPos = glm::ivec3(m_vPosition) * (int)CHUNK_SIZE;
    //unsigned nRand = rand() % 100 + 1;
    const SimplexNoise generator = SimplexNoise();
    const float fIncrementSize = 1000.0f;
    int nSeed = m_pWorld->GetWorldSeed();
    int wx = 0, wy = 0, wz = 0;
    for (int x = 0; x < CHUNK_SIZE; x++)
    {   
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            //wz = z + vPos.z;
            //nWorldHeight = glm::simplex(glm::vec2(wx, wz) * 0.01f) * 32 + 32;
            //nWorldHeight = GetHeight(x, z);
            //nLocalHeight = glm::min(nWorldHeight - vPos.y, (int)CHUNK_SIZE);
            //for (int y = 0; y < nLocalHeight; y++)
            wx = x + vChunkPos.x;
            wz = z + vChunkPos.z;
            float fMaxHeight = generator.fractal(7, (wx + nSeed) / fIncrementSize, (wz + nSeed) / fIncrementSize);
            fMaxHeight = (fMaxHeight + 1.0f) / 2.0f * (float)(CHUNK_SIZE * WORLD_H - 1);
            int maxHeight = (int)fMaxHeight;

            // float fStoneHeight = generator.fractal(7, (x + vChunkPos.x + nSeed) / fIncrementSize, (z + vChunkPos.z + nSeed) / fIncrementSize);
            // fStoneHeight = (fStoneHeight + 1.0f) / 2.0f * (float)(CHUNK_SIZE - 1);
            // int stoneHeight = (int)((fMaxHeight * fStoneHeight) / (float)(HALF_CHUNK_SIZE * WORLD_H - 1));
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                wy = y + vChunkPos.y;
                int nVoxelId = 0;
                //Underground may be cave
                if (wy < maxHeight - 1)
                {
                    if (generator.noise(wx * 0.09, y * 0.09, wz * 0.09) > 0 &&
                        generator.noise(wx * 0.1, wz * 0.1) * 3 + 3 < wy && wy < maxHeight -10)
                        nVoxelId = 0;
                    else
                        nVoxelId = STONE;
                }
                // else if (wy == maxHeight - 1)
                // {
                //     nVoxelId = rand() % 5 + 1;
                // }
                else if (wy == maxHeight - 1)
                {
                    nVoxelId = GRASS;
                }
                //std::cout << "nVoxelId: " << nVoxelId << std::endl;
                m_pVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = nVoxelId;
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
    vLayout.AddLayout<float>(3);
    vLayout.AddLayout<float>(2);
    vLayout.AddLayout<unsigned int>(1);
    vLayout.AddLayout<unsigned int>(1);

    m_ptrVertexArray->AddLayout(vLayout);
    m_bLoaded = true;
}
