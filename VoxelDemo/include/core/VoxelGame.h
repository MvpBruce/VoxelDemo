#pragma once
#include "Settings.h"

class Texture;
class World;
class Camera;
class VertexArray;
class VertexBuffer;

class VoxelGame
{
private:
    GLFWwindow* m_pGLFWwindow;
    std::shared_ptr<Texture> m_ptrTexture;
    std::shared_ptr<World> m_ptrWorld;

    glm::mat4 m_matModel;
    glm::mat4 m_matView;
    glm::mat4 m_matProject;

    float m_fLastTime;
    float m_fDeltaTime;
    Camera* m_pCamare;

    static float m_fLastX;
    static float m_fLastY;

public:
    VoxelGame(/* args */);
    ~VoxelGame();

    void Run();

private:
    bool InitGLW();
    void InitGame();
    void Handle_events();
    void Render();
    void Update();

    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};
