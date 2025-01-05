#pragma once
#include <glad/glad.h>

class IndexBuffer
{
private:
    unsigned int m_nRenderID;
    unsigned int m_nCount;

public:
    IndexBuffer() = default;
    IndexBuffer(unsigned int* pData, unsigned int nCount);
    ~IndexBuffer();

    void Bind();
    void Unbind();

    unsigned int GetCount();
};

IndexBuffer::IndexBuffer(unsigned int* pData, unsigned int nCount):
m_nCount(nCount)
{
    glGenBuffers(1, &m_nRenderID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nRenderID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pData), pData, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    Unbind();
}

inline void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nRenderID);
}

inline void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

inline unsigned int IndexBuffer::GetCount()
{
    return m_nCount;
}
