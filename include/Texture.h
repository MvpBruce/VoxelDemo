#pragma once

class Texture
{
private:
    unsigned int m_nTexture;

public:
    Texture(/* args */);
    ~Texture();

    bool LoadTexture(const char* pszPath);
    void Bind();
};
