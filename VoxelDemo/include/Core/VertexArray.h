#pragma once
#include <glad/glad.h>
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
private:
    unsigned int m_nRenderID;
    
public:
    VertexArray();
    VertexArray(VertexBufferLayout& vecLayout);
    ~VertexArray();

    void AddLayout(VertexBufferLayout& vecLayout);
    void Bind();
    void Unbind();
};