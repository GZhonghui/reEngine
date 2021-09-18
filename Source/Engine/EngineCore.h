#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"AssetManager.h"
#include"ProjectParser.h"

#include"VulkanManager.h"
#include"GLManager.h"

#include"Actor.h"
#include"Component.h"

#define RUN_ENGINE engineMain(initScene)

int engineMain(void (*initScene)(std::vector<std::shared_ptr<Actor>>* actorsInScene));

namespace EngineCore
{
    void initGLFW();
    void initImGuiForGL();
    void initOpenGL();

    void destroyGLFW();
    void destroyImGuiForGL();
    void destroyOpenGL();

    // EngineCoreEditorUI.cpp
    void RenderEditor();
    uint32_t RenderEditorScene(uint32_t viewWidth, uint32_t viewHeight);
    
    void RenderGame();
    
    void Render();

    void initActors();
    void destroyActors();

    void Update();

    void applyWorldSettings();
    void collectWorldSettings();

    void loadClassForRender();
};