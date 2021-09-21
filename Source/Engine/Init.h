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

#include"implClass_Car.h"

#include"implClass_Dog.h"

#include"implComponent_Moving.h"

void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
    {
        auto newActor = std::make_shared<Car>("Car_Default");
        newActor->insertTag("Actor_Car");
        newActor->setLocation(Eigen::Vector3d(-1.170000, -0.780000, -0.060000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 48.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(3.000000, 3.000000, 3.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<Dog>("Dog_Default");
        newActor->insertTag("Actor_Dog");
        newActor->addComponent(std::make_shared<Moving>("Dog_Default"));
        newActor->setLocation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 30.299999, 0.000000));
        newActor->setScale(Eigen::Vector3d(0.300000, 0.300000, 0.300000));
        actorsInScene->push_back(newActor);
    }
}
