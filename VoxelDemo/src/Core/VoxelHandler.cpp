#include "VoxelHandler.h"
#include "Settings.h"
#include "Camera.h"
#include "World.h"
#include "Chunk.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

unsigned int vertex[] = {
        0, 0, 1,  1, 0, 1,  1, 1, 1,  0, 1, 1, // front
        0, 1, 0,  0, 0, 0,  1, 0, 0,  1, 1, 0 // back
    };

unsigned int indices[] = {
        0, 2, 3,   0, 1, 2, // front
        1, 7, 2,   1, 6, 7, // right
        6, 5, 4,   4, 7, 6, // back
        3, 7, 4,   3, 2, 7, // top
        3, 4, 5,   3, 5, 0, // left
        0, 6, 1,   0, 5, 6  // bottom
    };

unsigned int texCoord[] = {
    0, 0, 1, 0, 1, 1, 0, 1
};

unsigned int texCoordIndices[] = {
    0, 2, 3, 0, 1, 2,
    0, 2, 3, 0, 1, 2,
    0, 1, 2, 2, 3, 0,
    0, 2, 3, 0, 1, 2, // top
    2, 3, 0, 2, 0, 1, // left
    3, 1, 2, 3, 0, 1
};

VoxelHandler::VoxelHandler(World* pWorld): m_pWorld(pWorld)
, m_nCurVoxelId(0), m_nCurVoxelIndex(-1), m_pChunk(nullptr),
m_nOperatorMode(1), m_bLoaded(false), m_pVB(std::make_shared<VertexBuffer>()),
m_pVA(std::make_shared<VertexArray>()),
m_pVertexData(nullptr), m_nVertexCount(0)
{
}

VoxelHandler::~VoxelHandler()
{
    if (m_pVertexData)
    {
        delete[] m_pVertexData;
        m_pVertexData = nullptr;
    }
}

void VoxelHandler::Active()
{
}

void VoxelHandler::Deactive()
{
    
}

bool VoxelHandler::MouseLButtonDown()
{
    if (m_nOperatorMode == 1)
        RemoveVoxel();
    else
        AddVoxel();
    
    return false;
}

bool VoxelHandler::MouseRbuttonDown()
{
    return false;
}

bool VoxelHandler::RayTrace()
{
    float tMaxX, tMaxY, tMaxZ, tDeltaX, tDeltaY, tDeltaZ;
    glm::ivec3 vCurPos(GetCamera()->GetPosition());

    // start point
    glm::vec3 vStart(GetCamera()->GetPosition());
    // end point
    glm::vec3 vEnd(vStart + GetCamera()->GetDirection() * MAX_RAY_DISTANCE);

    int dx = glm::sign(vEnd.x - vStart.x);
    if (dx != 0) tDeltaX = fmin(dx / (vEnd.x - vStart.x), 10000000.0f); else tDeltaX = 10000000.0f;
    if (dx > 0) tMaxX = tDeltaX * (1.0 - glm::fract(vStart.x)); else tMaxX = tDeltaX * glm::fract(vStart.x);

    int dy = glm::sign(vEnd.y - vStart.y);
    if (dy != 0) tDeltaY = fmin(dy / (vEnd.y - vStart.y), 10000000.0f); else tDeltaY = 10000000.0f;
    if (dy > 0) tMaxY = tDeltaY * (1.0 - glm::fract(vStart.y)); else tMaxY = tDeltaY * glm::fract(vStart.y);

    int dz = glm::sign(vEnd.z - vStart.z);
    if (dz != 0) tDeltaZ = fmin(dz / (vEnd.z - vStart.z), 10000000.0f); else tDeltaZ = 10000000.0f;
    if (dz > 0) tMaxZ = tDeltaZ * (1.0 - glm::fract(vStart.z)); else tMaxZ = tDeltaZ * glm::fract(vStart.z);

    m_vNormal = glm::ivec3(0);
    m_vCurWroldPos = glm::ivec3(0);
    Reset();

    //x=0, y=1, z=2
    int nDir = -1;
    while (true) 
    {
        if (GetChunkByPos(vCurPos) && m_nCurVoxelId)
        {
            m_vCurWroldPos = vCurPos;
            switch (nDir)
            {
            case 0:
                m_vNormal.x = -dx;
                break;
            case 1:
                m_vNormal.y = -dy;
                break;
            case 2:
                m_vNormal.z = -dz;
                break;
            default:
                break;
            }

            //std::cout << "detected: Pos:[" << m_vCurWroldPos.x << "," << m_vCurWroldPos.y << "," << m_vCurWroldPos.z << "], Voxel index: " << m_nCurVoxelIndex << std::endl;
            return true;
        }

        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) 
            {
                vCurPos.x += dx;
                tMaxX += tDeltaX;
                nDir = 0;
            } 
            else 
            {
                vCurPos.z += dz;
                tMaxZ += tDeltaZ;
                nDir = 2;
            }
        } 
        else 
        {
            if (tMaxY < tMaxZ)
            {
                vCurPos.y += dy;
                tMaxY += tDeltaY;
                nDir = 1;
            } 
            else
            {
                vCurPos.z += dz;
                tMaxZ += tDeltaZ;
                nDir = 2;
            }
        }

        if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
            break;
    }

    return false;
}

void VoxelHandler::Render()
{
    if (m_nCurVoxelId == 0)
        return;

    PrepareData();
    glm::ivec3 vTarget;
    if (m_nOperatorMode == 0)
        vTarget = m_vCurWroldPos + m_vNormal;
    else
        vTarget = m_vCurWroldPos;

    m_pVA->Bind();
    
    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(vTarget));
    ShaderManager::GetInstance().GetShader("cube")->Use();
    ShaderManager::GetInstance().GetShader("cube")->SetMatrix("model", model);
    ShaderManager::GetInstance().GetShader("cube")->SetInt("nOperation", m_nOperatorMode);
    
    //CALLERROR(glDrawElements(GL_TRIANGLES, m_pIB->GetCount(), GL_UNSIGNED_INT, indices));
    CALLERROR(glDrawArrays(GL_TRIANGLES, 0, m_nVertexCount));
}

void VoxelHandler::SetOperatorMode(int nType)
{
    m_nOperatorMode = nType;
}

int VoxelHandler::GetOperatorMode()
{
    return m_nOperatorMode;
}

bool VoxelHandler::GetChunkByPos(glm::ivec3 &vWorldPos)
{
    Reset();
    //Get pos in world index
    glm::vec3 vPos((float)vWorldPos.x, (float)vWorldPos.y, (float)vWorldPos.z);
    vPos /= (int)CHUNK_SIZE;
    if (vPos.x < 0 || vPos.x >= WORLD_W ||  vPos.y < 0 || vPos.y >= WORLD_H || 
        vPos.z < 0 || vPos.z >= WORLD_D)
    {
        return false;
    }

    glm::ivec3 vChunkPos(vWorldPos / (int)CHUNK_SIZE);
    //Get chunk by index
    unsigned int nWroldIndex = vChunkPos.x + vChunkPos.z * WORLD_D + vChunkPos.y * WORLD_AREA;
    Chunk* pChunk = m_pWorld->GetChunkByIndex(nWroldIndex);
    if (!pChunk)
        return false;

    //Get local positon
    glm::ivec3 vLocal = vWorldPos - vChunkPos * (int)CHUNK_SIZE;
    int nVoexlIndex = vLocal.x + vLocal.z * CHUNK_SIZE + vLocal.y * CHUNK_AREA;
    if (nVoexlIndex < 0)
        return false;

    m_pChunk = pChunk;
    m_nCurVoxelIndex = nVoexlIndex;
    m_nCurVoxelId = pChunk->GetChunkId(nVoexlIndex);
    m_vCurLocalPos = vLocal;
    return true;
}

void VoxelHandler::RemoveVoxel()
{
    //if m_nCurVoxelId is 0, empty
    if (m_nCurVoxelId && m_pChunk && m_nCurVoxelIndex >= 0)
    {
        //Set 0 to delete
        m_pChunk->SetVoxelIdByIndex(m_nCurVoxelIndex, 0);
        std::cout << "Deleted: WorldPos:[" << m_vCurWroldPos.x << "," << m_vCurWroldPos.y << "," << m_vCurWroldPos.z << "], Voxel index: " << m_nCurVoxelIndex << std::endl;
        
        //Rebuild chunk mesh
        m_pChunk->BuildChunkMesh();
        RebuildAdjacentChunks();
        Reset();
    }
}

void VoxelHandler::AddVoxel()
{
    if (m_nCurVoxelId)
    {
        glm::ivec3 vPos = m_vCurWroldPos + m_vNormal;
        //Empty place
        if (GetChunkByPos(vPos) && m_nCurVoxelId == 0)
        {
            m_pChunk->SetVoxelIdByIndex(m_nCurVoxelIndex, 1);
            m_pChunk->BuildChunkMesh();
            Reset();
        }
    }
}

void VoxelHandler::Reset()
{
    m_pChunk = nullptr;
    m_nCurVoxelIndex = -1;
    m_nCurVoxelId = 0;
}

void VoxelHandler::PrepareData()
{
    if (m_bLoaded)
        return;

    if (m_pVertexData)
        return;

    m_nVertexCount = sizeof(indices)/sizeof(unsigned int);
    //position + uv: 3 2
    m_pVertexData = new float[m_nVertexCount * 5];
    memset(m_pVertexData, 0, sizeof(float) * (m_nVertexCount * 5));
    unsigned int nOffSet = 0;
    for (int i = 0; i < m_nVertexCount; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m_pVertexData[nOffSet] = vertex[indices[i] * 3 + j];
            nOffSet++;
        }

        for (int k = 0; k < 2; k++)
        {
            m_pVertexData[nOffSet] = texCoord[texCoordIndices[i] * 2 + k];
            nOffSet++;
        }
    }
    
    m_pVB->AttachData(m_pVertexData, sizeof(float) * (m_nVertexCount * 5));

    VertexBufferLayout vLayout;
    vLayout.AddLayout<float>(3);
    vLayout.AddLayout<float>(2);
    m_pVA->AddLayout(vLayout);

    m_bLoaded = true;
}

void VoxelHandler::RebuildAdjacentChunks()
{
    if (!m_pChunk)
    {
        std::cout << "Can't rebuild adjacent chunks" << std::endl;
        return;
    }

    glm::ivec3 vRebuildPos;
    
    if (m_vCurLocalPos.x == 0)
    {
        vRebuildPos = m_vCurWroldPos;
        vRebuildPos.x -= 1;
        RebuildAdjacentChunk(vRebuildPos);
    }
    
    if (m_vCurLocalPos.x == CHUNK_SIZE - 1)
    {
        vRebuildPos = m_vCurWroldPos;
        vRebuildPos.x += 1;
        RebuildAdjacentChunk(vRebuildPos);
    }

    if (m_vCurLocalPos.y == 0)
    {
        vRebuildPos = m_vCurWroldPos;
        vRebuildPos.y -= 1;
        RebuildAdjacentChunk(vRebuildPos);
    }

    if (m_vCurLocalPos.y == CHUNK_SIZE - 1)
    {
        vRebuildPos = m_vCurWroldPos;
        vRebuildPos.y += 1;
        RebuildAdjacentChunk(vRebuildPos);
    }

    if (m_vCurLocalPos.z == 0)
    {
        vRebuildPos = m_vCurWroldPos;
        vRebuildPos.z -= 1;
        RebuildAdjacentChunk(vRebuildPos);
    }

    if (m_vCurLocalPos.z == CHUNK_SIZE - 1)
    {
        vRebuildPos = m_vCurWroldPos;
        vRebuildPos.z += 1;
        RebuildAdjacentChunk(vRebuildPos);
    }
}

int GetIndexInWorld(glm::vec3 vWorld)
{
    float cx = vWorld.x / (float)CHUNK_SIZE;
    float cy = vWorld.y / (float)CHUNK_SIZE;
    float cz = vWorld.z / (float)CHUNK_SIZE;

    if (cx < 0 || cx >= WORLD_W || cy < 0 || cy >= WORLD_H || cz < 0 || cz >= WORLD_D)
        return -1;

    return ((int)cx + (int)cz * WORLD_D + (int)cy * WORLD_AREA);
}

void VoxelHandler::RebuildAdjacentChunk(glm::ivec3& vWorldPos)
{
    //get chunk and rebuild
    int nIndex = GetIndexInWorld(glm::vec3(vWorldPos));
    if (nIndex == -1)
        return;

    Chunk* pChunk = m_pWorld->GetChunkByIndex(nIndex);
    if (!pChunk)
        return;

    pChunk->BuildChunkMesh();
}
