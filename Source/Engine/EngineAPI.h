#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"AssetManager.h"
#include"ProjectParser.h"

#include"VulkanManager.h"
#include"GLManager.h"

#include"Actor.h"
#include"Component.h"

namespace EngineCore
{
    std::shared_ptr<Actor> getActorByID(unsigned int actorID);
    std::shared_ptr<Actor> getActorByName(const std::string& actorName);
    std::vector<std::shared_ptr<Actor>> getActorsByTag(const std::string& tagName);
    std::vector<std::shared_ptr<Actor>> getActorsByClass(const std::string& className);
};