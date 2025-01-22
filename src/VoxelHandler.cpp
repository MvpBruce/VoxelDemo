#include "VoxelHandler.h"
#include "Settings.h"
#include "Camera.h"
#include "World.h"
#include "Chunk.h"

VoxelHandler::VoxelHandler(World* pWorld): m_pWorld(pWorld)
, m_nCurVoxelId(0), m_nCurVoxelIndex(-1), m_pChunk(nullptr)
{
}

VoxelHandler::~VoxelHandler()
{
}

void VoxelHandler::Active()
{
}

void VoxelHandler::Deactive()
{
}

bool VoxelHandler::MouseLButtonDown()
{
    RemoveVoxel();
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

    while (true) 
    {
        if (GetChunkByPos(vCurPos))
        {
            m_vCurrentPos = vCurPos;
            return true;
        }

        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) 
            {
                vCurPos.x += dx;
                tMaxX += tDeltaX;
            } 
            else 
            {
                vCurPos.z += dz;
                tMaxZ += tDeltaZ;
            }
        } 
        else 
        {
            if (tMaxY < tMaxZ)
            {
                vCurPos.y += dy;
                tMaxY += tDeltaY;
            } 
            else
            {
                vCurPos.z += dz;
                tMaxZ += tDeltaZ;
            }
        }

        if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
            break;
    }
    
    return false;
}

bool VoxelHandler::GetChunkByPos(glm::ivec3 &vWorldPos)
{
    //Check world pos
    // if (vWorldPos.x < 0 || vWorldPos.x >= CHUNK_SIZE * WORLD_W || vWorldPos.y < 0
    //     || vWorldPos.y >= WORLD_H * CHUNK_SIZE || vWorldPos.z < 0 || vWorldPos.z >= CHUNK_SIZE * WORLD_D)
    // {
    //     return false;
    // }
    
    //Get pos in world index
    glm::ivec3 vChunkPos(vWorldPos / (int)CHUNK_SIZE);
    if (vChunkPos.x < 0 || vChunkPos.x >= WORLD_W ||  vChunkPos.y < 0 || vChunkPos.y >= WORLD_H || 
        vChunkPos.z < 0 || vChunkPos.z >= WORLD_D)
    {
        return false;
    }

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

    int nVoxelId = pChunk->GetChunkId(nVoexlIndex);
    if (nVoxelId == 0)
        return false;
    
    m_pChunk = pChunk;
    m_nCurVoxelIndex = nVoexlIndex;
    m_nCurVoxelId = nVoxelId;
    return true;
}

void VoxelHandler::RemoveVoxel()
{
    //if m_nCurVoxelId is 0, empty
    if (m_nCurVoxelId && m_pChunk && m_nCurVoxelIndex >= 0)
    {
        //Set 0 to delete
        m_pChunk->SetVoxelIdByIndex(m_nCurVoxelIndex, 0);
        std::cout << "To be deleted: " << m_nCurVoxelIndex << std::endl;
        
        //Rebuild chunk mesh
        m_pChunk->BuildChunkMesh();
        m_pChunk = nullptr;
        m_nCurVoxelId = 0;
        m_nCurVoxelIndex = -1;
    }
}
