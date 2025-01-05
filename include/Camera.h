#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    glm::vec3 m_vFront;
    glm::vec3 m_vUp;
    glm::vec3 m_vRight;
    glm::vec3 m_vPivot;
    glm::vec3 m_vWorldUp;
    glm::mat4 m_matView;

public:
    Camera(const glm::vec3& vPos, const glm::vec3& vAt, const glm::vec3& vUp);
    ~Camera();

    glm::mat4& GetViewMatrix();

    void ProcessKeyBoard(Direction dir, float fDelta);
    void ProcessMouseMovement(float offsetX, float offsetY);
    void ProcessMouseWheelScroll(float offsetX, float offsetY);

private:
    void UpdateCamera();
};

Camera::Camera(const glm::vec3& vPos, const glm::vec3& vFront, const glm::vec3& vUp):
m_vPos(vPos),
m_vFront(vFront),
m_vUp(vUp),
m_vPivot(0.0f),
m_vWorldUp(vUp),
m_matView(1.0f)
{
    m_vRight = glm::normalize(glm::cross(m_vFront, m_vUp));
}

Camera::~Camera()
{
}

inline glm::mat4 &Camera::GetViewMatrix()
{
    m_matView = glm::lookAt(m_vPos, m_vPos + m_vFront, m_vUp);
    return m_matView;
}

inline void Camera::ProcessKeyBoard(Direction dir, float fDelta)
{
    switch (dir)
    {
    case Direction::Left:
        m_vPos -= fDelta * m_vRight;
        break;
    case Direction::Right:
        m_vPos += fDelta * m_vRight;
        break;
    case Direction::Forward:
        m_vPos += fDelta * m_vFront;
        break;
    case Direction::Backward:
        m_vPos -= fDelta * m_vFront;
        break;
    case Direction::Up:
        m_vPos += fDelta * m_vUp;
        break;
    case Direction::Down:
        m_vPos -= fDelta * m_vUp;
        break;
    default:
        break;
    }
}

inline void Camera::ProcessMouseMovement(float offsetX, float offsetY)
{
    // if (glm::abs(glm::dot(m_vUp, m_vWorldUp)) < 0.1f)
    //     return;

    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::rotate(mat, glm::radians(offsetX), glm::vec3(0.0f, 1.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(offsetY), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat3 mat3x3(mat);

    glm::vec3 vPos = mat3x3 * m_vPos;
    glm::vec3 vFront = glm::normalize(m_vPivot - vPos);
    glm::vec3 vRight = glm::normalize(glm::cross(vFront, m_vWorldUp));
    glm::vec3 vUp = glm::normalize(glm::cross(vRight, vFront));
    if (glm::abs(glm::dot(m_vUp, m_vWorldUp)) < 0.1f)
        return;

    m_vPos = vPos;
    m_vFront = vFront;
    m_vRight = vRight;
    m_vUp = vUp;
}

void Camera::ProcessMouseWheelScroll(float offsetX, float offsetY)
{
    ProcessKeyBoard(Direction::Forward, offsetY);
}

inline void Camera::UpdateCamera()
{
}
