#include "VoxelGame.h"
#include "World.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <iostream>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VoxelHandler.h"
#include "ShaderManager.h"

float VoxelGame::m_fLastX = -1.0f;
float VoxelGame::m_fLastY = -1.0f;

std::shared_ptr<VoxelHandler> g_ptrVoxelHandler;

VoxelGame::VoxelGame():
m_fLastTime(0.0f), m_fDeltaTime(0.0f), 
m_ptrTexture(nullptr), m_ptrWorld(nullptr), m_pCamare(nullptr)
{
    InitGLW();
    InitGame();
}

VoxelGame::~VoxelGame()
{
}

void VoxelGame::Run()
{
    while (!glfwWindowShouldClose(m_pGLFWwindow))
    {
        Update();
        Handle_events();
        Render();
    }

    glfwTerminate();
}

bool VoxelGame::InitGLW()
{
    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    m_pGLFWwindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLDemo", nullptr, nullptr);
    if (m_pGLFWwindow == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_pGLFWwindow);
    glfwSetFramebufferSizeCallback(m_pGLFWwindow, VoxelGame::framebuffer_size_callback);
    glfwSetCursorPosCallback(m_pGLFWwindow, VoxelGame::mouseCallback);
    glfwSetMouseButtonCallback(m_pGLFWwindow, VoxelGame::mouseButtonCallback);
    glfwSetScrollCallback(m_pGLFWwindow, VoxelGame::mouseScrollCallback);
    glfwSetInputMode(m_pGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //GLFW_CURSOR_NORMAL //GLFW_CURSOR_DISABLED

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    std::cout << glGetString(GL_VERSION) << std::endl;

    return true;
}

void VoxelGame::InitGame()
{
    m_pCamare = GetCamera();
    ShaderManager::GetInstance().CreateShader("cube")->Use();
    ShaderManager::GetInstance().GetShader("cube")->SetMatrix("project", m_pCamare->GetProjectMatrix());

    ShaderManager::GetInstance().CreateShader("chunk")->Use();

    if (!m_ptrTexture)
        m_ptrTexture = std::make_shared<Texture>();
    
    if (!m_ptrWorld)
        m_ptrWorld = std::make_shared<World>();

    m_ptrWorld->BuildChunkMesh();
    g_ptrVoxelHandler = std::make_shared<VoxelHandler>(m_ptrWorld.get());
    ShaderManager::GetInstance().GetShader("chunk")->SetMatrix("project", m_pCamare->GetProjectMatrix());
}

void VoxelGame::Handle_events()
{
    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_pGLFWwindow, true);

    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_W) == GLFW_PRESS)
        m_pCamare->ProcessKeyBoard(Direction::Forward, m_fDeltaTime);

    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_S) == GLFW_PRESS)
        m_pCamare->ProcessKeyBoard(Direction::Backward, m_fDeltaTime);
    
    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_A) == GLFW_PRESS)
        m_pCamare->ProcessKeyBoard(Direction::Left, m_fDeltaTime);

    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_D) == GLFW_PRESS)
        m_pCamare->ProcessKeyBoard(Direction::Right, m_fDeltaTime);
    
    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_Q) == GLFW_PRESS)
        m_pCamare->ProcessKeyBoard(Direction::Up, m_fDeltaTime);
    
    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_E) == GLFW_PRESS)
        m_pCamare->ProcessKeyBoard(Direction::Down, m_fDeltaTime);

    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_R) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_V) == GLFW_PRESS)
        glfwSetInputMode(m_pGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_B) == GLFW_PRESS)
        glfwSetInputMode(m_pGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_T) == GLFW_PRESS)
        g_ptrVoxelHandler->SetOperatorMode(0);

    if(glfwGetKey(m_pGLFWwindow, GLFW_KEY_G) == GLFW_PRESS)
        g_ptrVoxelHandler->SetOperatorMode(1);
}

void VoxelGame::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderManager::GetInstance().GetShader("cube")->Use();
    ShaderManager::GetInstance().GetShader("cube")->SetInt("u_texture", 0);
    ShaderManager::GetInstance().GetShader("cube")->SetMatrix("view", m_pCamare->GetViewMatrix());

    
    ShaderManager::GetInstance().GetShader("chunk")->Use();
    ShaderManager::GetInstance().GetShader("chunk")->SetInt("textureArray", 1);
    ShaderManager::GetInstance().GetShader("chunk")->SetMatrix("view", m_pCamare->GetViewMatrix());

    m_ptrTexture->ActiveAndBind();

    m_ptrWorld->Render();
    g_ptrVoxelHandler->Render();
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(m_pGLFWwindow);
    glfwPollEvents();
}

void VoxelGame::Update()
{
    float fCurTime = static_cast<float>(glfwGetTime());
    m_fDeltaTime = fCurTime - m_fLastTime;
    m_fLastTime = fCurTime;
    m_pCamare->Update();
    g_ptrVoxelHandler->RayTrace();
}

void VoxelGame::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    float fPosX = static_cast<float>(xpos);
    float fPosY = static_cast<float>(ypos);
    
    if (m_fLastX == -1.0 && m_fLastY == -1.0)
    {
        m_fLastX = fPosX;
        m_fLastY = fPosY;
        return;
    }

    float fOffsetX = fPosX - m_fLastX;
    float fOffsetY = fPosY - m_fLastY;

    m_fLastX = fPosX;
    m_fLastY = fPosY;
    
    GetCamera()->ProcessMouseMovement(fOffsetX, fOffsetY);
}

void VoxelGame::mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    GetCamera()->ProcessMouseWheelScroll(xoffset, yoffset);
}

void VoxelGame::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void VoxelGame::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
            g_ptrVoxelHandler->MouseLButtonDown();   
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            g_ptrVoxelHandler->MouseRbuttonDown();
    }
}
