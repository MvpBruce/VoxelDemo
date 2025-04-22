#pragma once
#include <glad/glad.h>

class IndexBuffer
{
private:
    unsigned int m_nRenderID;
    unsigned int m_nCount;

public:
    IndexBuffer();
    ~IndexBuffer();

    void AttachData(unsigned int* pData, unsigned int nCount);
    void Bind();
    void Unbind();

    unsigned int GetCount();
};


