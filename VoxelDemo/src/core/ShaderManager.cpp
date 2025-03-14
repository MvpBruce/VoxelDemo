#include "ShaderManager.h"
#include "Shader.h"

ShaderManager &ShaderManager::GetInstance()
{
    static ShaderManager shaderMgr;
    return shaderMgr;
}

Shader* ShaderManager::CreateShader(const char *pszShaderName)
{
    std::shared_ptr<Shader> pShader = std::make_shared<Shader>(pszShaderName);
    m_mapShader.emplace(pszShaderName, pShader);
    return pShader.get();
}

Shader *ShaderManager::GetShader(const char *pszShaderName)
{
    auto it = m_mapShader.find(pszShaderName);
    if (it != m_mapShader.end())
        return it->second.get();

    return nullptr;
}
