#pragma once

class Texture
{
private:
    unsigned int m_nTexture;
    unsigned int m_nTexture1;
    unsigned int m_nTextureArray;

public:
    Texture(/* args */);
    ~Texture();

    bool LoadTexture(const char* pszPath, bool bIsArray = false);
    void ActiveAndBind();
};
