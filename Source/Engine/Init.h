/*
=== Default Code ===

#pragma once

#include<vector>
#include<memory>

#include"Actor.h"
#include"Component.h"

void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
}
*/

#pragma once

#include<vector>
#include<memory>

#include"Actor.h"
#include"Component.h"

#include"implBee.h"

void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
    {
        auto newActor = std::make_shared<Bee>("Bee_01");
        newActor->setLocation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(2.000000, 2.000000, 2.000000));
        actorsInScene->push_back(newActor);
    }
}
