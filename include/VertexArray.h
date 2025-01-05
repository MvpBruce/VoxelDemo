#pragma once
#include <glad/glad.h>
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
private:
    unsigned int m_nRenderID;
    
public:
    VertexArray() = default;
    VertexArray(VertexBufferLayout& vecLayout);
    ~VertexArray();

    void Bind();
    void Unbind();
};

VertexArray::VertexArray(VertexBufferLayout& vecLayout)
{
    glGenVertexArrays(1, &m_nRenderID);
    Bind();
    std::vector<VertexElemets> vecElements = vecLayout.GetElemets();
    unsigned int nOffset = 0;
    for (unsigned int i = 0; i < vecElements.size(); i ++)
    {
        auto& ele = vecElements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, ele.nCount, ele.nType, ele.bNormalized, vecLayout.GetStride(), (const void*)nOffset);
        nOffset += sizeof(ele.nType) * ele.nCount;
    }
}

VertexArray::~VertexArray()
{
    Unbind();
}

inline void VertexArray::Bind()
{
    glBindVertexArray(m_nRenderID);
}

inline void VertexArray::Unbind()
{
    glBindVertexArray(0);
}
