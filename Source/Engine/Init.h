/*
=== Default Code ===

#pragma once

#include<vector>
#include<memory>

#include"Actor.h"

void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
}
*/

#pragma once

#include<vector>
#include<memory>

#include"Actor.h"

#include"implManager.h"

#include"implBox.h"

#include"implBee.h"

void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
    {
        auto newActor = std::make_shared<Manager>("mainManager");
        newActor->insertTag("Manager_01");
        newActor->setLocation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<Bee>("Bee_01");
        newActor->insertTag("Bee");
        newActor->setLocation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<Box>("Box_01");
        newActor->insertTag("Box");
        newActor->setLocation(Eigen::Vector3d(3.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(3.000000, 3.000000, 3.000000));
        actorsInScene->push_back(newActor);
    }
}
