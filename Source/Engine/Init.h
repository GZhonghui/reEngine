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

#include"implClass_Ground.h"

#include"implClass_B_Normal.h"

#include"implClass_B_NS.h"

#include"implClass_B.h"

#include"implClass_B_Glass.h"

#include"implClass_B_Metal.h"

#include"implClass_B_Cartoon.h"

#include"implClass_House.h"

#include"implClass_Tree.h"

#include"implClass_Dog.h"

#include"implComponent_Moving.h"

void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
    {
        auto newActor = std::make_shared<UserClass::Ground>("Ground_Default");
        newActor->insertTag("Actor_Ground");
        newActor->setLocation(Eigen::Vector3d(0.000000, -0.520000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::B_Normal>("B_Normal_Default");
        newActor->insertTag("Actor_B_Normal");
        newActor->setLocation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::B_NS>("B_NS_Default");
        newActor->insertTag("Actor_B_NS");
        newActor->addComponent(std::make_shared<UserComponent::Moving>("B_NS_Default"));
        newActor->setLocation(Eigen::Vector3d(3.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::B>("B_Default");
        newActor->insertTag("Actor_B");
        newActor->setLocation(Eigen::Vector3d(-3.000000, 0.000000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::B_Glass>("B_Glass_Default");
        newActor->insertTag("Actor_B_Glass");
        newActor->setLocation(Eigen::Vector3d(0.000000, 3.400000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::B_Metal>("B_Metal_Default");
        newActor->insertTag("Actor_B_Metal");
        newActor->setLocation(Eigen::Vector3d(-3.000000, 3.400000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::B_Cartoon>("B_Cartoon_Default");
        newActor->insertTag("Actor_B_Cartoon");
        newActor->setLocation(Eigen::Vector3d(3.000000, 3.400000, 0.000000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 0.000000, 0.000000));
        newActor->setScale(Eigen::Vector3d(1.000000, 1.000000, 1.000000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::House>("House_Default");
        newActor->insertTag("Actor_House");
        newActor->setLocation(Eigen::Vector3d(-10.200000, -0.550000, 4.880000));
        newActor->setRotation(Eigen::Vector3d(0.000000, -142.199997, 0.000000));
        newActor->setScale(Eigen::Vector3d(0.100000, 0.100000, 0.100000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::Tree>("Tree_Default");
        newActor->insertTag("Actor_Tree");
        newActor->setLocation(Eigen::Vector3d(8.920000, 0.620000, 4.810000));
        newActor->setRotation(Eigen::Vector3d(0.000000, 32.099998, 0.000000));
        newActor->setScale(Eigen::Vector3d(0.800000, 0.800000, 0.800000));
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<UserClass::Dog>("Dog_Default");
        newActor->insertTag("Actor_Dog");
        newActor->setLocation(Eigen::Vector3d(-6.430000, 0.000000, -3.020000));
        newActor->setRotation(Eigen::Vector3d(0.000000, -69.800003, 0.000000));
        newActor->setScale(Eigen::Vector3d(0.300000, 0.300000, 0.300000));
        actorsInScene->push_back(newActor);
    }
}
