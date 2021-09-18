#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"GLMisc.h"

class GLRenderable
{
public:
    GLRenderable()
    {
        m_VAOID = 0;
        m_VBOID = 0;
        m_EBOID = 0;

        m_EnableDiffuseTexture  = false;
        m_EnableNormalTexture   = false;
        m_EnableSpecularTexture = false;

        m_DiffuseTextureID  = 0;
        m_NormalTextureID   = 0;
        m_SpecularTextureID = 0;
    }
    virtual ~GLRenderable()
    {
        Clear();
    }

protected:
    uint32_t m_VAOID;
    uint32_t m_VBOID;
    uint32_t m_EBOID;

    Color m_DiffuseColor;

    bool m_EnableDiffuseTexture;
    bool m_EnableNormalTexture;
    bool m_EnableSpecularTexture;

    uint32_t m_DiffuseTextureID;
    uint32_t m_NormalTextureID;
    uint32_t m_SpecularTextureID;

    double m_N;

protected:
    std::string m_Shader;

    unsigned int m_ElementCount;

public:
    void  setDiffuseColor(const Color& newColor) { m_DiffuseColor = newColor; }
    Color getDiffuseColor() const noexcept       { return m_DiffuseColor;     }

    void   setN(double newN) { m_N = newN; }
    double getN() const      { return m_N; }

    void setShader(const std::string& newShader) { m_Shader = newShader; }

    uint32_t getDiffuseTextureID()  const { return m_DiffuseTextureID;  }
    uint32_t getNormalTextureID()   const { return m_NormalTextureID;   }
    uint32_t getSpeculartextureID() const { return m_SpecularTextureID; }

public:
    void reLoadModel(const std::string& Model);
    void reLoadDiffuseTexture (const std::string& DiffuseTextureName);
    void reLoadNormalTexture  (const std::string& NormalTextureName);
    void reLoadSpecularTexture(const std::string& SpecularTextureName);

    void Clear();

public:
    void Draw();

public:
    friend class GLManager;
};