#pragma once
#include <glad/glad.h>

class VertexBuffer
{
private:
    unsigned int m_nRenderID;

public:
    VertexBuffer() = default;
    VertexBuffer(void* pData, unsigned int nSize);
    ~VertexBuffer();

    void Bind();
    void Unbind();
};

VertexBuffer::VertexBuffer(void* pData, unsigned int nSize)
{
    glGenBuffers(1, &m_nRenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_nRenderID);
    glBufferData(GL_ARRAY_BUFFER, nSize, pData, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    Unbind();
}

inline void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_nRenderID);
}

inline void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
