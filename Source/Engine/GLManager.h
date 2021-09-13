#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"GLRenderable.h"

class GLManager
{
public:
    GLManager() = default;
    ~GLManager() = default;

protected:
    unsigned int m_SceneFBO;
    unsigned int m_SceneTextureID;
    unsigned int m_DepthTextureID;

    uint32_t m_ViewWidth;
    uint32_t m_ViewHeight;
    
    Point m_CameraLocation;
    Direction m_CameraDir;

// Light
protected:
    Direction m_LightDir;
    Color     m_LightColor;
    double    m_LightPower;

protected:
    void InitLight();

public:
    Direction getLightDir() const noexcept { return m_LightDir; }
    void      setLightDir(const Direction& newDir) { m_LightDir = newDir; }

    Color     getLightColor() const noexcept { return m_LightColor; }
    void      setLightColor(const Color& newColor) { m_LightColor = newColor; }
    
    double    getLightPower() const noexcept { return m_LightPower; }
    void      setLightPower(double newPower) { m_LightPower = newPower; }
// Light

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
    void RenderSkybox();
// Skybox

// Default Scene
protected:
    uint32_t m_DefaultSceneTextureID;
    uint32_t m_DefaultSceneShaderProgramID;
    uint32_t m_DefaultSceneVAOID;
    uint32_t m_DefaultSceneVBOID;

protected:
    void InitDefaultScene();
    void DestroyDefaultScene();

public:
    void RenderDefaultScene();
// Default Scene

public:
    bool Init();
    bool Destroy();

public:
    void Render(std::shared_ptr<GLRenderable> renderObj, const glm::mat4& matModel);

public:
    unsigned int getSceneTextureID() const { return m_SceneTextureID; }

public:
    void BeginRenderEditor(uint32_t viewWidth, uint32_t viewHeight,
        const Point& mainCameraLocation, const Direction& mainCameraDir);
    void EndRenderEditor();

    void BeginRenderGame(uint32_t viewWidth, uint32_t viewHeight,
        const Point& mainCameraLocation, const Direction& mainCameraDir);
    void EndRenderGame();
};