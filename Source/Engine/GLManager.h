#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

class GLManager
{
public:
    GLManager() = default;
    ~GLManager() = default;

protected:
    unsigned int m_SceneFBO;
    unsigned int m_SceneTextureID;

public:
    bool Init();
    bool Destroy();

public:
    unsigned int RenderScene(uint32_t viewWidth, uint32_t viewHeight);
    void RenderGame(uint32_t viewWidth, uint32_t viewHeight);
};