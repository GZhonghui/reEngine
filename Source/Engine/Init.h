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

#include"implClass_Cat.h"

#include"implComponent_Moving.h"

void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
    {
        auto newActor = std::make_shared<Car>("Car_Default");
        newActor->insertTag("Actor_Car");
        newActor->setLocation(Eigen::Vector3d(0.000000, -2.280000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(3.000000, 3.000000, 3.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<Cat>("Cat_Default");
        newActor->insertTag("Actor_Cat");
        newActor->setLocation(Eigen::Vector3d(14.870000, -1.340000, 5.520000));
        newActor->setRotation(Eigen::Vector3d(0.000000, -52.900002, 0.000000));
        newActor->setScale(Eigen::Vector3d(0.200000, 0.200000, 0.200000));
        actorsInScene->push_back(newActor);
    }
}
