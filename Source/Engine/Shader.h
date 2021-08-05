#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

class Shader
{
protected:
    std::vector<char> m_ShaderCode;

public:
    Shader(const char* filePath)
    {
        loadFrom(filePath);
    }

    ~Shader()
    {
        Destroy();
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
};