#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"Loader.h"
#include"Shader.h"

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
    uint32_t m_VAOID;
    uint32_t m_VBOID;
    uint32_t m_EBOID;

public:
    void Init();
    void Clear();

public:
    void Draw();
};