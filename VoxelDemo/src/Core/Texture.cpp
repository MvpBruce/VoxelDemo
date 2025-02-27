#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Settings.h"

Texture::Texture()
{
    stbi_set_flip_vertically_on_load(true);

    //Texture
    CALLERROR(glGenTextures(1, &m_nTexture));
    CALLERROR(glBindTexture(GL_TEXTURE_2D, m_nTexture));
    
    //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string strTextureDir = std::filesystem::current_path().string() + "/assets/textures/";
    std::string strImg = strTextureDir + "frame.png";
    if (!LoadTexture(strImg.c_str(), false))
    {  
        std::cout << "Failed to Texture: " << strImg << std::endl;
        return;
    }

    //Texture array
    glGenTextures(1, &m_nTextureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_nTextureArray);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    strImg = strTextureDir + "tex_array_0.png";
    if (!LoadTexture(strImg.c_str(), true))
    {
        std::cout << "Failed to Texture: " << strImg << std::endl;
        return;
    }
}

Texture::~Texture()
{
}

bool Texture::LoadTexture(const char *pszPath, bool bIsArray/* = false*/)
{
    int nTotalWidth, nTotalHeight, nChanel;
    int nReq_comp = 4;
    //Force rgba
    unsigned char* pData = stbi_load(pszPath, &nTotalWidth, &nTotalHeight, &nChanel, nReq_comp);
    if (!pData)
        return false;

    if (bIsArray)
    {

        int nLayers = 3 * nTotalHeight / nTotalWidth;
        int nHeight = nTotalHeight / nLayers;
        CALLERROR(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, nTotalWidth, nHeight, nLayers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        for (size_t i = 0; i < nLayers; i++)
        {
            unsigned char* pLayerData = pData + ((nLayers - 1 - i) * nHeight * nTotalWidth * nReq_comp);
            CALLERROR(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, nTotalWidth, nHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, pLayerData));
        }
    }
    else
    {
        CALLERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nTotalWidth, nTotalHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData));
        CALLERROR(glGenerateMipmap(GL_TEXTURE_2D));
    }

    stbi_image_free(pData);
    return true;
}

void Texture::ActiveAndBind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nTexture);

    glActiveTextureARB(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_nTextureArray);
}