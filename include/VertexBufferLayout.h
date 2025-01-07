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

    std::vector<VertexElemets>& GetElemets();

    template<typename T>
    void AddLayout(unsigned int nCount);

    // template<>
    // void AddLayout<float>(unsigned int nCount);

    // template<>
    // void AddLayout<unsigned int>(unsigned int nCount);

    // template<>
    // void AddLayout<unsigned char>(unsigned int nCount);

    unsigned int GetStride();
};

