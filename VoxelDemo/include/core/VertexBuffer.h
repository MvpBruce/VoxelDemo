#pragma once
#include <glad/glad.h>

class VertexBuffer
{
private:
    unsigned int m_nRenderID;

public:
    VertexBuffer();
    VertexBuffer(void* pData, unsigned int nSize);
    ~VertexBuffer();

    void AttachData(void* pData, unsigned int nSize);
    void Bind();
    void Unbind();
};
