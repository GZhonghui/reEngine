#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

enum class sType
{
    VERT, FRAG
};

class Shader
{
protected:
    std::vector<char> m_ShaderCode;

public:
    Shader() = default;
    Shader(const char* ShaderType, sType Stage)
    {
        Init(ShaderType, Stage);
    }

    virtual ~Shader()
    {
        Destroy();
    }

public:
    void Init(const char* ShaderType, sType Stage)
    {
        std::string ShaderFileName = "./Shader/" +
            std::string(ShaderType) + "." +
            (Stage == sType::VERT ? "vert" : (Stage == sType::FRAG ? "frag" : "NONE"));

        loadFrom(ShaderFileName.c_str());
    }

protected:
    bool loadFrom(const char* filePath)
    {
        FILE* shaderFile = nullptr;
        shaderFile = fopen(filePath, "rb");

        if (!shaderFile)
        {
            Out::Log(pType::ERROR, "Cant load shader file: %s", filePath);
            return false;
        }

        Destroy();

        char read = 0;
        while (EOF != fscanf(shaderFile, "%c", &read))
        {
            m_ShaderCode.push_back(read);
        }
        m_ShaderCode.push_back(0);

        fclose(shaderFile);
        return true;
    }

    bool Destroy()
    {
        m_ShaderCode.clear();
        return true;
    }

public:
    friend class VulkanManager;

    friend class GLManager;
    friend class GLRenderable;
};