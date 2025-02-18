#include "Frustum.h"
#include "Camera.h"

Frustum::Frustum()
{
    m_fVTan = glm::tan(V_FOV * 0.5);
    m_fVCosFactor = 1.0 / glm::cos(V_FOV * 0.5);
    m_fHTan = glm::tan(H_FOV * 0.5);
    m_fHCosFactor = 1.0 / glm::cos(H_FOV * 0.5);
}

Frustum::~Frustum()
{
}

bool Frustum::IsInside(glm::vec3& vCenter)
{
    Camera* pCamera = GetCamera();
    glm::vec3 vSphere = vCenter - pCamera->GetPosition();

    //Is between near and far plane?
    float fz = glm::dot(vSphere, pCamera->GetDirection());
    if (fz < Z_NEAR - CHUNK_RADIUS || fz > Z_FAR + CHUNK_RADIUS)
        return false;

    //Inside top and bottom?
    float fy = glm::dot(vSphere, pCamera->GetUp());
    float fDistance = m_fVTan * fz + m_fVCosFactor * CHUNK_RADIUS;
    if (fy < -fDistance || fy > fDistance)
        return false;

    //Inside left and right
    float fx = glm::dot(vSphere, pCamera->GetRight());
    fDistance = m_fHTan * fz + m_fHCosFactor * CHUNK_RADIUS;
    if (fx < -fDistance || fx > fDistance)
        return false;

    return true;
}
