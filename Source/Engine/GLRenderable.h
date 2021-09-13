#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"GLMisc.h"

class GLRenderable
{
public:
    GLRenderable() = default;
    virtual ~GLRenderable() = default;

protected:
    Shader m_ShaderVert;
    Shader m_ShaderFrag;

protected:
    uint32_t m_ShaderProgramID;
    uint32_t m_DiffuseTextureID;
    uint32_t m_VAOID;
    uint32_t m_VBOID;
    uint32_t m_EBOID;

    Color m_DiffuseColor;

    unsigned int m_ElementCount;

public:
    void Init(std::string Model, std::string DiffuseTexture, Color DiffuseColor);
    void Clear();

public:
    void Draw(glm::mat4* MVP);
};