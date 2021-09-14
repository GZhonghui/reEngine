#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"GLMisc.h"

class GLRenderable
{
private:
    bool m_NeedClear;
public:
    GLRenderable()
    {
        m_NeedClear = false;
    }
    virtual ~GLRenderable()
    {
        Clear();
    }

protected:
    uint32_t m_ShaderProgramID;
    uint32_t m_DiffuseTextureID;
    uint32_t m_VAOID;
    uint32_t m_VBOID;
    uint32_t m_EBOID;

    Color m_DiffuseColor;

    unsigned int m_ElementCount;

public:
    unsigned int getShaderID() const { return m_ShaderProgramID; }
    unsigned int getDiffuseTextureID() const { return m_DiffuseTextureID; }

public:
    void Init(const std::string& Model, const std::string& DiffuseTexture, Color DiffuseColor);
    void Clear();

public:
    void Draw();
};