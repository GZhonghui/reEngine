#pragma once

#include<vector>
#include<memory>

#include"Actor.h"

#include"implnoRender.h"

#include"implhasRender.h"

#include"implthirdClass.h"

inline void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
    {
        auto newActor = std::make_shared<hasRender>("firstRenderObj");
        newActor->insertTag("hasRender");
        newActor->setLocation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
}
