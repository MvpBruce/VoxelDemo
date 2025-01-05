

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include "../include/Shader.h"
#include "../include/VertexBuffer.h"
#include "../include/IndexBuffer.h"
#include "../include/VertexArray.h"
#include "Settings.h"
#include "Player.hpp"
#include "Chunk.hpp"
#include "Texture.hpp"


//Player
Player player(PALYER_POS);

Chunk chunk;
//end
class VoxelEngie
{
public:
    VoxelEngie(): m_fLastTime(0.0f), m_fDeltaTime(0.0f)
    {
        Init();
    }

    ~VoxelEngie() 
    {
        if (m_pTexture) delete m_pTexture;
    }

private:
    bool Init()
    {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // glfw window creation
        // --------------------
        m_pGLFWwindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLDemo", NULL, NULL);
        if (m_pGLFWwindow == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_pGLFWwindow);
        glfwSetFramebufferSizeCallback(m_pGLFWwindow, VoxelEngie::framebuffer_size_callback);
        glfwSetCursorPosCallback(m_pGLFWwindow, VoxelEngie::mouseCallback);
        //glfwSetMouseButtonCallback(m_pGLFWwindow, VoxelEngie::mouseButtonCallback);
        glfwSetScrollCallback(m_pGLFWwindow, VoxelEngie::mouseScrollCallback);
        glfwSetInputMode(m_pGLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        glEnable(GL_DEPTH_TEST);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        std::cout << glGetString(GL_VERSION) << std::endl;

        m_shader = Shader("res/shaders/chunk.vs", "res/shaders/chunk.fs");
    
        //create a vertex buffer object
        m_VertexBuffer = new VertexBuffer(chunk.GetData(), chunk.GetSize());

        //create vertex index buffer object
        //m_IndexBuffer = new IndexBuffer(indices, sizeof(indices)/sizeof(unsigned int));

        //create a vertex array object
        VertexBufferLayout vLayout;
        vLayout.AddLayout<unsigned int>(3);
        vLayout.AddLayout<unsigned int>(1);
        vLayout.AddLayout<unsigned int>(1);
        
        m_VertexArray = new VertexArray(vLayout);

        m_pTexture = new Texture;
        m_pTexture->LoadTexture("res/textures/test.png");

        m_shader.Use();
        //m_shader.SetInt("texture", 0);  
        //m_shader.SetInt("texture2", 1);

        //model
        m_matModel = glm::mat4(1.0f);
        m_shader.SetMatrix("model", m_matModel);

        //view
        m_matView = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        m_shader.SetMatrix("view", m_matView);

        //project
        m_shader.SetMatrix("project", player.GetProjectMatrix());
        return true;
    }

    void Update()
    {
        float fCurTime = static_cast<float>(glfwGetTime());
        m_fDeltaTime = fCurTime - m_fLastTime;
        m_fLastTime = fCurTime;
    }

    void Render()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //for (int i = 0; i < vTextures.size(); i++)
        // {
        //     glActiveTexture(GL_TEXTURE0 + i);
        //     glBindTexture(GL_TEXTURE_2D, vTextures[i]);
        // }
        
        m_VertexArray->Bind();
        m_shader.Use();
        player.Update();
        //for (int i = 0; i < 10; i++)
        {
            //modelMat = glm::translate(modelMat, cubePositions[2]);
            //glm::mat4 tm = glm::translate(m_matModel, m_pCubePositions[i]);
            //m_shader.SetMatrix("model", tm);
            m_shader.SetMatrix("view", player.GetViewMatrix());
            //m_shader.SetMatrix("project", m_matProject);
            //glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, indices);
            glDrawArrays(GL_TRIANGLES, 0, chunk.GetCount());
        }
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_pGLFWwindow);
        glfwPollEvents();
    }

    void Handle_envets()
    {
        processInput(m_pGLFWwindow);
    }

    void processInput(GLFWwindow *window)
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            player.ProcessKeyBoard(Direction::Forward, m_fDeltaTime);

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            player.ProcessKeyBoard(Direction::Backward, m_fDeltaTime);
        
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            player.ProcessKeyBoard(Direction::Left, m_fDeltaTime);

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            player.ProcessKeyBoard(Direction::Right, m_fDeltaTime);
        
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            player.ProcessKeyBoard(Direction::Up, m_fDeltaTime);
        
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            player.ProcessKeyBoard(Direction::Down, m_fDeltaTime);
    }

    static void mouseCallback(GLFWwindow *window, double xpos, double ypos)
    {
        // if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
        //     return;

        float fPosX = static_cast<float>(xpos);
        float fPosY = static_cast<float>(ypos);
        

        float fOffsetX = fPosX - m_fLastX;
        float fOffsetY = fPosY - m_fLastY;

        m_fLastX = fPosX;
        m_fLastY = fPosY;
        
        player.ProcessMouseMovement(fOffsetX, fOffsetY);
    }

    // static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    // {
    //     if (button == GLFW_MOUSE_BUTTON_LEFT)
    //     {
    //         if (action == GLFW_RELEASE)
    //             bFirstMove = true;
    //     }
    // }

    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        player.ProcessMouseWheelScroll(xoffset, yoffset);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

public:
    void Run()
    {
        while (!glfwWindowShouldClose(m_pGLFWwindow))
        {
            Update();
            Handle_envets();
            Render();
        }

        glfwTerminate();
    }

private:
    GLFWwindow* m_pGLFWwindow;
    Shader m_shader;
    VertexArray* m_VertexArray;
    VertexBuffer* m_VertexBuffer;
    IndexBuffer* m_IndexBuffer;
    float* m_pVertices;
    unsigned int* m_pIndices;
    glm::vec3* m_pCubePositions;
    glm::mat4 m_matModel;
    glm::mat4 m_matView;
    glm::mat4 m_matProject;
    float m_fLastTime;
    float m_fDeltaTime;
    static float m_fLastX;
    static float m_fLastY;
    Texture* m_pTexture;
};

float VoxelEngie::m_fLastX = SCR_WIDTH / 2;
float VoxelEngie::m_fLastY = SCR_HEIGHT / 2;

int main()
{
    VoxelEngie v;
    v.Run();
}


