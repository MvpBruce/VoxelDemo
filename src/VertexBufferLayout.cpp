#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout(/* args */):
m_nStride(0)
{

}

VertexBufferLayout::~VertexBufferLayout()
{
}

std::vector<VertexElemets>& VertexBufferLayout::GetElemets()
{
    return m_Elements;
}

unsigned int VertexBufferLayout::GetStride()
{
    return m_nStride;
}

template<typename T>
void VertexBufferLayout::AddLayout(unsigned int nCount)
{

}

template<>
void VertexBufferLayout::AddLayout<float>(unsigned int nCount)
{
    m_Elements.push_back({nCount, GL_FLOAT, GL_FALSE});
    m_nStride += nCount * sizeof(GL_FLOAT);
}

template<>
void VertexBufferLayout::AddLayout<unsigned int>(unsigned int nCount)
{
    m_Elements.push_back({nCount, GL_UNSIGNED_INT, GL_FALSE});
    m_nStride += nCount * sizeof(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::AddLayout<unsigned char>(unsigned int nCount)
{
    m_Elements.push_back({nCount, GL_UNSIGNED_BYTE, GL_TRUE});
    m_nStride += nCount * sizeof(GL_UNSIGNED_BYTE);
}