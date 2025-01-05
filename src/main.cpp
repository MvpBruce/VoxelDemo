#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include "../include/Shader.h"
#include "../include/VertexBuffer.h"
#include "../include/IndexBuffer.h"
#include "../include/VertexArray.h"
#include "Camera.h"

#define CALLERROR(func)\
        func;\
        unsigned int nError = glGetError();\
        if (nError != GL_NO_ERROR)\
        {\
            std::cout << "[OpenGL ERROR]: " << nError << " " << #func << " " << __FILE__ << " " << __LINE__ << std::endl;\
        }\

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

//camera
glm::vec3 vPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 vFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(vPos, vFront, vUp);


float fLastX = SCR_WIDTH/2.0f;
float fLastY = SCR_HEIGHT/2.0f;
bool bFirstMove = true;
//vertex
struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 uv;
};

struct Quad
{
    std::vector<Vertex> vecVertexs;
    std::vector<int> indicas;
};

//test data
float vertices[] = {
            // Positions          // Colors          // Texture Coords
            // Back face
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left-back
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right-back
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-right-back
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-left-back

            // Front face
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom-left-front
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom-right-front
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Top-right-front
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // Top-left-front

            // Left face
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left-back
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-left-front
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-left-front
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-left-back

            // Right face
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-right-back
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right-front
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-right-front
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-right-back

            // Bottom face
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left-back
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right-back
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Bottom-right-front
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Bottom-left-front

            // Top face
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Top-left-back
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Top-right-back
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-right-front
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // Top-left-front
        };

        unsigned int indices[] = {
            // Back face
            0, 1, 2, 2, 3, 0,

            // Front face
            4, 5, 6, 6, 7, 4,

            // Left face
            8, 9, 10, 10, 11, 8,

            // Right face
            12, 13, 14, 14, 15, 12,

            // Bottom face
            16, 17, 18, 18, 19, 16,

            // Top face
            20, 21, 22, 22, 23, 20
        };

        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f), 
            glm::vec3( 2.0f,  5.0f, -15.0f), 
            glm::vec3(-1.5f, -2.2f, -2.5f),  
            glm::vec3(-3.8f, -2.0f, -12.3f),  
            glm::vec3( 2.4f, -0.4f, -3.5f),  
            glm::vec3(-1.7f,  3.0f, -7.5f),  
            glm::vec3( 1.3f, -2.0f, -2.5f),  
            glm::vec3( 1.5f,  2.0f, -2.5f), 
            glm::vec3( 1.5f,  0.2f, -1.5f), 
            glm::vec3(-1.3f,  1.0f, -1.5f)  
        };
//end 

class VoxelEngie
{
public:
    VoxelEngie(): m_fLastTime(0.0f), m_fDelta(0.0f)
    {
        Init();
    }

    ~VoxelEngie() 
    {
        // if (m_pVertices) delete[] m_pVertices;
        // if (m_pIndices) delete[] m_pIndices;
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
        glfwSetMouseButtonCallback(m_pGLFWwindow, VoxelEngie::mouseButtonCallback);
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

        std::cout << glGetString(GL_VERSION) << std::endl;

        m_shader = Shader("res/shaders/vertex.vs", "res/shaders/vertex.fs");

        // float vertices[] = {
        //     // Positions          // Colors          // Texture Coords
        //     // Back face
        //     -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left-back
        //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right-back
        //     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-right-back
        //     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-left-back

        //     // Front face
        //     -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom-left-front
        //     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom-right-front
        //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Top-right-front
        //     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // Top-left-front

        //     // Left face
        //     -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left-back
        //     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-left-front
        //     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-left-front
        //     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-left-back

        //     // Right face
        //     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-right-back
        //     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right-front
        //     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-right-front
        //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-right-back

        //     // Bottom face
        //     -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left-back
        //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right-back
        //     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Bottom-right-front
        //     -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Bottom-left-front

        //     // Top face
        //     -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Top-left-back
        //     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Top-right-back
        //     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-right-front
        //     -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // Top-left-front
        // };

        // m_pVertices = new float[sizeof(vertices)/sizeof(float)];
        // memcpy(m_pVertices, vertices, sizeof(m_pVertices));

        // unsigned int indices[] = {
        //     // Back face
        //     0, 1, 2, 2, 3, 0,

        //     // Front face
        //     4, 5, 6, 6, 7, 4,

        //     // Left face
        //     8, 9, 10, 10, 11, 8,

        //     // Right face
        //     12, 13, 14, 14, 15, 12,

        //     // Bottom face
        //     16, 17, 18, 18, 19, 16,

        //     // Top face
        //     20, 21, 22, 22, 23, 20
        // };

        // m_pIndices = new unsigned int[sizeof(indices)/sizeof(unsigned int)];
        // memcpy(m_pIndices, indices, sizeof(m_pIndices));
    
        //create a vertex buffer object
        m_VertexBuffer = VertexBuffer(vertices, sizeof(vertices));

        //create vertex index buffer object
        m_IndexBuffer = IndexBuffer(indices, sizeof(indices)/sizeof(unsigned int));

        //create a vertex array object
        m_vtLayout.AddLayout<float>(3);
        m_vtLayout.AddLayout<float>(3);
        m_vtLayout.AddLayout<float>(2);
        
        m_VertexArray = VertexArray(m_vtLayout);

        //create texture buffer object
        unsigned int nTexture1;
        glGenTextures(1, &nTexture1);
        glBindTexture(GL_TEXTURE_2D, nTexture1);
        vTextures.push_back(nTexture1);

        //set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //load texture
        stbi_set_flip_vertically_on_load(true);
        int nWidth, nHeight, nChanel;
        unsigned char* pData = stbi_load("res/textures/container.jpg", &nWidth, &nHeight, &nChanel, 0);
        if (pData)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Fail to open image: " << stbi_failure_reason() << std::endl;
        }

        stbi_image_free(pData);

        //second texture
        unsigned int nTexture2;
        glGenTextures(1, &nTexture2);
        glBindTexture(GL_TEXTURE_2D, nTexture2);
        vTextures.push_back(nTexture2);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //load texture
        pData = stbi_load("res/textures/awesomeface.png", &nWidth, &nHeight, &nChanel, 0);
        if (pData)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Fail to open image: " << stbi_failure_reason() << std::endl;
        }

        stbi_image_free(pData);

        m_shader.Use();
        m_shader.SetInt("texture1", 0);  
        m_shader.SetInt("texture2", 1);

        //model
        m_matModel = glm::mat4(1.0f);
        m_shader.SetMatrix("model", m_matModel);

        //view
        m_matView = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        m_shader.SetMatrix("view", m_matView);

        //project
        m_matProject = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        m_shader.SetMatrix("project", m_matProject);
    
        // glm::vec3 cubePositions[] = {
        //     glm::vec3( 0.0f,  0.0f,  0.0f), 
        //     glm::vec3( 2.0f,  5.0f, -15.0f), 
        //     glm::vec3(-1.5f, -2.2f, -2.5f),  
        //     glm::vec3(-3.8f, -2.0f, -12.3f),  
        //     glm::vec3( 2.4f, -0.4f, -3.5f),  
        //     glm::vec3(-1.7f,  3.0f, -7.5f),  
        //     glm::vec3( 1.3f, -2.0f, -2.5f),  
        //     glm::vec3( 1.5f,  2.0f, -2.5f), 
        //     glm::vec3( 1.5f,  0.2f, -1.5f), 
        //     glm::vec3(-1.3f,  1.0f, -1.5f)  
        // };

        // m_pCubePositions = new glm::vec3[sizeof(cubePositions)/sizeof(glm::vec3)];
        // memcpy(m_pCubePositions, cubePositions, sizeof(m_pCubePositions));

        return true;
    }

    void Update()
    {
        float fCurTime = static_cast<float>(glfwGetTime());
        m_fDelta = fCurTime - m_fLastTime;
        m_fLastTime = fCurTime;
    }

    void Render()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < vTextures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, vTextures[i]);
        }
        
        m_VertexArray.Bind();
        m_shader.Use();

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 tm = glm::translate(m_matModel, m_pCubePositions[i]);
            m_shader.SetMatrix("model", tm);
            //m_shader.SetMatrix("model", m_matModel);
            m_shader.SetMatrix("view", camera.GetViewMatrix());
            m_shader.SetMatrix("project", m_matProject);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, indices);
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
            camera.ProcessKeyBoard(Direction::Forward, m_fDelta);

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyBoard(Direction::Backward, m_fDelta);
        
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyBoard(Direction::Left, m_fDelta);

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyBoard(Direction::Right, m_fDelta);
        
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.ProcessKeyBoard(Direction::Up, m_fDelta);
        
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.ProcessKeyBoard(Direction::Down, m_fDelta);
    }

    static void mouseCallback(GLFWwindow *window, double xpos, double ypos)
    {
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
            return;


        float fPosX = static_cast<float>(xpos);
        float fPosY = static_cast<float>(ypos);
        
        if (bFirstMove)
        {
            fLastX = fPosX;
            fLastY = fPosY;
            bFirstMove = false;
        }
        
        float fOffsetX = (fLastX - fPosX) * 0.2f;
        float fOffsetY = (fLastY - fPosY) * 0.2f;

        fLastX = fPosX;
        fLastY = fPosY;
        
        camera.ProcessMouseMovement(fOffsetX, fOffsetY);
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_RELEASE)
                bFirstMove = true;
        }
    }

    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.ProcessMouseWheelScroll(xoffset, yoffset);
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
    VertexArray m_VertexArray;
    VertexBuffer m_VertexBuffer;
    VertexBufferLayout m_vtLayout;
    IndexBuffer m_IndexBuffer;
    std::vector<unsigned int> vTextures;
    float* m_pVertices;
    unsigned int* m_pIndices;
    glm::vec3* m_pCubePositions;
    glm::mat4 m_matModel;
    glm::mat4 m_matView;
    glm::mat4 m_matProject;
    float m_fLastTime;
    float m_fDelta;
};

int main()
{
    VoxelEngie v;
    v.Run();
}


