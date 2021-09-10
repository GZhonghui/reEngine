#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"Loader.h"
#include"Shader.h"

class GLManager
{
public:
    GLManager() = default;
    ~GLManager() = default;

protected:
    unsigned int m_SceneFBO;
    unsigned int m_SceneTextureID;

// Skybox
protected:
    uint32_t m_SkyboxTextureID;
    uint32_t m_SkyboxShaderProgramID;
    uint32_t m_SkyboxVAOID;
    uint32_t m_SkyboxVBOID;

protected:
    void InitSkybox();
    void DestroySkybox();

public:
    void RenderSkybox(uint32_t viewWidth, uint32_t viewHeight,
        const Point& mainCameraLocation, const Direction& mainCameraDir);
// Skybox

public:
    bool Init();
    bool Destroy();

public:
    unsigned int getSceneTextureID() const { return m_SceneTextureID; }

public:
    void BeginRenderEditor(uint32_t viewWidth, uint32_t viewHeight);
    void EndRenderEditor();

    void BeginRenderGame(uint32_t viewWidth, uint32_t viewHeight);
    void EndRenderGame();
};