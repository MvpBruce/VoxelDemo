#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_nRenderID);
}

VertexArray::VertexArray(VertexBufferLayout &vecLayout)
{
    glGenVertexArrays(1, &m_nRenderID);
    AddLayout(vecLayout);
}

VertexArray::~VertexArray()
{
    Unbind();
    glDeleteVertexArrays(1, &m_nRenderID);
}

void VertexArray::AddLayout(VertexBufferLayout &vecLayout)
{
    Bind();
    std::vector<VertexElemets> vecElements = vecLayout.GetElemets();
    unsigned int nOffset = 0;
    for (unsigned int i = 0; i < vecElements.size(); i ++)
    {
        auto& ele = vecElements[i];
        glEnableVertexAttribArray(i);

        if (ele.nType == GL_FLOAT)
        {
            glVertexAttribPointer(i, ele.nCount, ele.nType, ele.bNormalized, vecLayout.GetStride(), (const void*)nOffset);
        }
        else
        {
            glVertexAttribIPointer(i, ele.nCount, ele.nType, vecLayout.GetStride(), (const void*)nOffset);
        }
        
        nOffset += sizeof(ele.nType) * ele.nCount;
    }
}

void VertexArray::Bind()
{
    glBindVertexArray(m_nRenderID);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}
