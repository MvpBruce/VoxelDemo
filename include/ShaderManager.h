#pragma once
#include <unordered_map>
#include <memory>

class Shader;
class ShaderManager
{
public:
    static ShaderManager& GetInstance();
    Shader* CreateShader(const char* pszShaderName);
    Shader* GetShader(const char* pszShaderName);

private:
    ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator = (const ShaderManager&) = delete;
    ~ShaderManager() = default;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_mapShader;
};
