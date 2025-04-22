#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_nRenderID);
}

IndexBuffer::~IndexBuffer()
{
    Unbind();
}

void IndexBuffer::AttachData(unsigned int *pData, unsigned int nCount)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nRenderID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pData), pData, GL_STATIC_DRAW);
    m_nCount = nCount;
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nRenderID);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetCount()
{
    return m_nCount;
}