#include "VertexBuffer.h"
#include "Settings.h"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_nRenderID);
}

VertexBuffer::VertexBuffer(void *pData, unsigned int nSize)
{
    glGenBuffers(1, &m_nRenderID);
    AttachData(pData, nSize);
}

void VertexBuffer::AttachData(void* pData, unsigned int nSize)
{
    CALLERROR(glBindBuffer(GL_ARRAY_BUFFER, m_nRenderID));
    CALLERROR(glBufferData(GL_ARRAY_BUFFER, nSize, pData, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    Unbind();
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_nRenderID);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}