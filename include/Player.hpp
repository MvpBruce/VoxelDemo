#pragma once

#include "Camera.hpp"

class Player : public Camera
{
public:
    Player(glm::vec3 vPos = PALYER_POS, float fYaw = -90.0f, float fPitch = 0.0f):
    Camera(vPos, fYaw, fPitch)
    {

    }
    ~Player() {}
};
