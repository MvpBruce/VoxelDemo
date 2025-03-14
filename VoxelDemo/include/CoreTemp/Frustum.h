#pragma once

#include "Settings.h"

class Frustum
{
private:
    float m_fVTan;
    float m_fVCosFactor;
    float m_fHTan;
    float m_fHCosFactor;

public:
    Frustum(/* args */);
    ~Frustum();

    bool IsInside(glm::vec3& vCenter);
};
