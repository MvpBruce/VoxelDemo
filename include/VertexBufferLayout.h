#pragma once
#include <vector>
#include <glad/glad.h>

struct VertexElemets
{
    unsigned int nCount;
    unsigned int nType;
    bool bNormalized;
};

class VertexBufferLayout
{
private:
    std::vector<VertexElemets> m_Elements;
    unsigned int m_nStride;

public:
    VertexBufferLayout(/* args */);
    ~VertexBufferLayout();

    std::vector<VertexElemets>& GetElemets()
    {
        return m_Elements;
    }

    template<typename T>
    void AddLayout(unsigned int nCount)
    {

    }

    // template<>
    // void AddLayout<float>(unsigned int nCount);

    // template<>
    // void AddLayout<unsigned int>(unsigned int nCount);

    // template<>
    // void AddLayout<unsigned char>(unsigned int nCount);

    unsigned int GetStride()
    {
        return m_nStride;
    }
};

VertexBufferLayout::VertexBufferLayout(/* args */):
m_nStride(0)
{

}

VertexBufferLayout::~VertexBufferLayout()
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