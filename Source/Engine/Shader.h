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
    std::string m_ShaderType;
    sType m_ShaderStage;

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
        m_ShaderType = std::string(ShaderType);

        if (m_ShaderType == "GLDefault")
        {
            if (Stage == sType::VERT)
            {
                loadFrom("./Shader/GLDefault.vert");
            }
            else if (Stage == sType::FRAG)
            {
                loadFrom("./Shader/GLDefault.frag");
            }
        }
        else if (m_ShaderType == "GLSkybox")
        {
            if (Stage == sType::VERT)
            {
                loadFrom("./Shader/GLSkybox.vert");
            }
            else if (Stage == sType::FRAG)
            {
                loadFrom("./Shader/GLSkybox.frag");
            }
        }
        else if (m_ShaderType == "VKDefault")
        {
            if (Stage == sType::VERT)
            {
                loadFrom("./Shader/VKDefault.vert");
            }
            else if (Stage == sType::FRAG)
            {
                loadFrom("./Shader/VKDefault.frag");
            }
        }
        else
        {
            Out::Log(pType::ERROR, "Wrong Shader Type : %s", ShaderType);
        }
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