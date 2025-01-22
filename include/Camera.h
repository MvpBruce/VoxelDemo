#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Settings.h"

enum class Direction
{
    Left,
    Right,
    Forward,
    Backward,
    Up,
    Down
};

class Camera
{
private:
    glm::vec3 m_vPos;
    glm::vec3 m_vForward;
    glm::vec3 m_vUp;
    glm::vec3 m_vRight;
    //glm::vec3 m_vPivot;
    //glm::vec3 m_vWorldUp;
    
    float m_fYaw;
    float m_fPitch;
    glm::mat4 m_matProject;
    glm::mat4 m_matView;

public:
    Camera(glm::vec3 vPosition = PALYER_POS, float fYaw = -90.0f, float fPitch = 0.0f);
    //Camera(const glm::vec3& vPos, const glm::vec3& vAt, const glm::vec3& vUp);
    ~Camera();

    glm::mat4& GetViewMatrix();
    glm::mat4& GetProjectMatrix();
    glm::vec3& GetPosition();
    glm::vec3& GetDirection();

    void Update();
    void RotatePitch(float fDeltaY);
    void RotateYaw(float fDeltaX);
    void ProcessKeyBoard(Direction dir, float fDelta);
    //void ProcessMouseMovement(float offsetX, float offsetY);
    void ProcessMouseMovement(float offsetX, float offsetY);
    void ProcessMouseWheelScroll(float offsetX, float offsetY);

private:
    void UpdateVectors();
    void UpdateViewMatrix();
};

Camera* GetCamera(int nIndex = 0);




