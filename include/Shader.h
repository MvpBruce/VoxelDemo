#ifndef __SHADER_H
#define __SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const char* pszName)
    {
        Load(pszName);
    }

    void Load(const char* pszName)
    {
        std::string shaderPath = std::string("res/shaders/") + pszName + ".vs";
        std::string vs = ReadShader(shaderPath.c_str());
        shaderPath = std::string("res/shaders/") + pszName + ".fs";
        std::string fs = ReadShader(shaderPath.c_str());
        const char* pszVS = vs.c_str();
        const char* pszFS = fs.c_str();
        unsigned int nID = glCreateProgram();
        unsigned int vsShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fsShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vsShader, 1, &pszVS, nullptr);
        glShaderSource(fsShader, 1, &pszFS, nullptr);
        glCompileShader(vsShader);
        CheckCompileError(vsShader);
        glCompileShader(fsShader);
        CheckCompileError(fsShader);

        glAttachShader(nID, vsShader);
        glAttachShader(nID, fsShader);
        glLinkProgram(nID);
        glValidateProgram(nID);

        glDeleteShader(vsShader);
        glDeleteShader(fsShader);

        m_nID = nID;
    }

    void Use()
    {
        glUseProgram(m_nID);
    }

    unsigned int GetProgramID()
    {
        return m_nID;
    }

    void SetInt(const char* pszName, int nValue)
    {
        unsigned int nLocation = glGetUniformLocation(m_nID, pszName);
        glUniform1i(nLocation, nValue);
    }

    void SetMatrix(const char* pszName, glm::mat4& mat)
    {
        unsigned int nLocation = glGetUniformLocation(m_nID, pszName);
        glUniformMatrix4fv(nLocation, 1, GL_FALSE, glm::value_ptr(mat));
    }

private:
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    std::string ReadShader(const char* pFile)
    {
        std::ifstream ifs;
        ifs.open(pFile);
        std::ostringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        return ss.str();
    }

    bool CheckCompileError(unsigned int nShader)
    {
        int nParams;
        glGetShaderiv(nShader, GL_COMPILE_STATUS, &nParams);
        if (nParams == GL_FALSE)
        {
            int nLength;
            glGetShaderiv(nShader, GL_INFO_LOG_LENGTH, &nLength);
            char* pszMessage = (char*)alloca(nLength * sizeof(char));
            glGetShaderInfoLog(nShader, nLength * sizeof(char), &nLength, pszMessage);
            std::cout << pszMessage << std::endl;
            glDeleteShader(nShader);
        }

        return nParams;
    }

private:
    unsigned int m_nID;
};

#endif