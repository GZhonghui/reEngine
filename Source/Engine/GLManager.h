#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

#include"GLRenderable.h"
#include"Event.h"

class GLManager
{
public:
    GLManager() = default;
    ~GLManager() = default;

private:
    const float NearZ  = 0.1f;
    const float FarZ   = 300.0f;
    const float Aspect = 60.0f;

protected:
    unsigned int m_SceneFBO;
    unsigned int m_SceneTextureID;
    unsigned int m_DepthTextureID;

    // Update Every Frame
    uint32_t m_ViewWidth;
    uint32_t m_ViewHeight;
    
    Point     m_CameraLocation;
    Direction m_CameraDir;

// Light
protected:
    Direction m_LightDir;
    Color     m_LightColor;
    double    m_LightPower;

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

// Grid
protected:
    GLRenderable m_Grid;

protected:
    void InitGrid();
    void DestroyGrid();

public:
    // 修改了这里
    // 如果是使用模型渲染一个默认的地面 就使用Renderable
    // 如果是绘制线条 就改回自己的Shader和VAO
    void RenderGrid();
// Grid

// Axis
protected:
    uint32_t m_AxisEndShaderProgramID;
    uint32_t m_AxisEndVAOID;
    uint32_t m_AxisEndVBOID;

    uint32_t m_AxisLineShaderProgramID;
    uint32_t m_AxisLineVAOID;
    uint32_t m_AxisLineVBOID;

protected:
    void InitAxis();
    void DestroyAxis();

public:
    void RenderAxis(double Length, double Size, glm::mat4* MVP);
// Axis

public:
    bool Init();
    bool Destroy();

public:
    unsigned int getSceneTextureID() const { return m_SceneTextureID; }

public:
    void BeginRenderEditor(uint32_t viewWidth, uint32_t viewHeight,
        const Point& mainCameraLocation, const Direction& mainCameraDir);
    void EndRenderEditor();

    void BeginRenderGame(uint32_t viewWidth, uint32_t viewHeight,
        const Point& mainCameraLocation, const Direction& mainCameraDir);
    void EndRenderGame();

public:
    void Render(std::shared_ptr<GLRenderable> renderObj,
        const Transform& ObjTransform, bool Selected, bool LineMode);
};