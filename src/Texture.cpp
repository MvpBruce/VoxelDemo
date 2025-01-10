#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Settings.h"

Texture::Texture()
{
    glGenTextures(1, &m_nTexture);
    Bind();
    //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
}

Texture::~Texture()
{
}

bool Texture::LoadTexture(const char *pszPath)
{
    int nWidth, nHeight, nChanel;
    unsigned char* pData = stbi_load(pszPath, &nWidth, &nHeight, &nChanel, 0);
    if (pData)
    {
        CALLERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pData));
        CALLERROR(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        return false;
    }

    stbi_image_free(pData);
    return true;
}

void Texture::Bind()
{
    CALLERROR(glBindTexture(GL_TEXTURE_2D, m_nTexture));
}
