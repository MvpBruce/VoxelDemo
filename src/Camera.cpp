#include "Camera.h"

Camera::Camera(glm::vec3 vPosition, float fYaw, float fPitch)
: m_vPos(vPosition), m_fYaw(fYaw), m_fPitch(fPitch),
m_vForward(FRONT), m_vUp(UP), m_vRight(RIGHT)
{
    m_matProject = glm::perspective(V_FOV, ASPECT_RATIO, Z_NEAR, Z_FAR);
}

Camera::~Camera()
{
}

glm::mat4 &Camera::GetViewMatrix()
{
    return m_matView;
}

glm::mat4& Camera::GetProjectMatrix()
{
    return m_matProject;
}

void Camera::Update()
{
    UpdateVectors();
    UpdateViewMatrix();
}

void Camera::RotatePitch(float fDeltaY)
{
    m_fPitch -= fDeltaY;
    m_fPitch = glm::clamp(m_fPitch, -PITCH_MAX, PITCH_MAX);
}

void Camera::RotateYaw(float fDeltaX)
{
    m_fYaw += fDeltaX;
}

void Camera::ProcessKeyBoard(Direction dir, float fDelta)
{
    switch (dir)
    {
    case Direction::Left:
        m_vPos -= fDelta * m_vRight * PLAYER_SPEED;
        break;
    case Direction::Right:
        m_vPos += fDelta * m_vRight * PLAYER_SPEED;
        break;
    case Direction::Forward:
        m_vPos += fDelta * m_vForward * PLAYER_SPEED;
        break;
    case Direction::Backward:
        m_vPos -= fDelta * m_vForward * PLAYER_SPEED;
        break;
    case Direction::Up:
        m_vPos += fDelta * m_vUp * PLAYER_SPEED;
        break;
    case Direction::Down:
        m_vPos -= fDelta * m_vUp * PLAYER_SPEED;
        break;
    default:
        break;
    }
}

void Camera::ProcessMouseWheelScroll(float offsetX, float offsetY)
{
    ProcessKeyBoard(Direction::Forward, offsetY * MOUSE_SENSITIVITY);
}

void Camera::ProcessMouseMovement(float offsetX, float offsetY)
{
    RotateYaw(offsetX * MOUSE_SENSITIVITY);
    RotatePitch(offsetY * MOUSE_SENSITIVITY);
}

void Camera::UpdateVectors()
{
    m_vForward.x = glm::cos(glm::radians(m_fYaw)) * glm::cos(glm::radians(m_fPitch));
    m_vForward.y = glm::sin(glm::radians(m_fPitch));
    m_vForward.z = glm::sin(glm::radians(m_fYaw)) * glm::cos(glm::radians(m_fPitch));
    m_vForward = glm::normalize(m_vForward);

    m_vRight = glm::normalize(glm::cross(m_vForward, UP));
    m_vUp = glm::normalize(glm::cross(m_vRight, m_vForward));
}

void Camera::UpdateViewMatrix()
{
    m_matView = glm::lookAt(m_vPos, m_vPos + m_vForward, m_vUp);
}

// void Camera::ProcessMouseMovement(float offsetX, float offsetY)
// {
//     // if (glm::abs(glm::dot(m_vUp, m_vWorldUp)) < 0.1f)
//     //     return;

//     glm::mat4 mat = glm::mat4(1.0f);
//     mat = glm::rotate(mat, glm::radians(offsetX), glm::vec3(0.0f, 1.0f, 0.0f));
//     mat = glm::rotate(mat, glm::radians(offsetY), glm::vec3(1.0f, 0.0f, 0.0f));
//     glm::mat3 mat3x3(mat);

//     glm::vec3 vPos = mat3x3 * m_vPos;
//     glm::vec3 vFront = glm::normalize(m_vPivot - vPos);
//     glm::vec3 vRight = glm::normalize(glm::cross(vFront, m_vWorldUp));
//     glm::vec3 vUp = glm::normalize(glm::cross(vRight, vFront));
//     if (glm::abs(glm::dot(m_vUp, m_vWorldUp)) < 0.1f)
//         return;

//     m_vPos = vPos;
//     m_vFront = vFront;
//     m_vRight = vRight;
//     m_vUp = vUp;
// }

// Camera::Camera(const glm::vec3& vPos, const glm::vec3& vFront, const glm::vec3& vUp):
// m_vPos(vPos),
// m_vFront(vFront),
// m_vUp(vUp),
// m_vPivot(0.0f),
// m_vWorldUp(vUp),
// m_matView(1.0f)
// {
//     m_vRight = glm::normalize(glm::cross(m_vFront, m_vUp));
// }
