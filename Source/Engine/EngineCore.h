#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"VulkanManager.h"
#include"GLManager.h"

#include"Actor.h"

#include"CodeTemplate.h"

#ifdef BUILD_GAME_MODE
#include"Init.h"
#endif

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

    void RenderEditorUI();
    void RenderEditorScene();
    void Render();

    void initActors();
    void destroyActors();

    void Update();

    std::shared_ptr<Actor> getActorByID(unsigned int actorID);
    std::shared_ptr<Actor> getActorByName(const std::string& actorName);
    std::vector<std::shared_ptr<Actor>> getActorsByTag(const std::string& tagName);
    std::vector<std::shared_ptr<Actor>> getActorsByClass(const std::string& className);
};