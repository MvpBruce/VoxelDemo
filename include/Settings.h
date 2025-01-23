#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

#define CALLERROR(func)\
        {\
        func;\
        unsigned int nError = glGetError();\
        if (nError != GL_NO_ERROR)\
        {\
            std::cout << "[OpenGL ERROR]: " << nError << " " << #func << " " << __FILE__ << " " << __LINE__ << std::endl;\
        }\
        }\

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

//Chunk
const unsigned int CHUNK_SIZE = 32;
const unsigned int HALF_CHUNK_SIZE = CHUNK_SIZE / 2;
const unsigned int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
const unsigned int CHUNK_VOL = CHUNK_AREA * CHUNK_SIZE;

//Camera
const float ASPECT_RATIO = SCR_WIDTH / SCR_HEIGHT;
const float V_FOV = glm::radians(50.0f);
const float Z_NEAR = 0.1f;
const float Z_FAR = 2000.0f;
const float PITCH_MAX = 89.0f;
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

//World
const unsigned int WORLD_W = 2;
const unsigned int WORLD_D = 2;
const unsigned int WORLD_H = 1;
const unsigned int WORLD_AREA = WORLD_W * WORLD_D;
const unsigned int WORLD_VOL = WORLD_AREA * WORLD_H;

//Player
const glm::vec3 PALYER_POS = glm::vec3(WORLD_W * HALF_CHUNK_SIZE, WORLD_H * CHUNK_SIZE/2, WORLD_D * CHUNK_SIZE);
//const glm::vec3 PALYER_POS = glm::vec3(0, 0, 5.0f);
const float MOUSE_SENSITIVITY = 0.09f;
const float PLAYER_SPEED = 10.0f;
const float MAX_RAY_DISTANCE = 6.0f;

