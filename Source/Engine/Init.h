#pragma once

#include<vector>
#include<memory>

#include"Actor.h"

#include"implCar.h"

inline void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
    {
        auto newActor = std::make_shared<Car>("Car_01");
        newActor->insertTag("Actor of Car");
        actorsInScene->push_back(newActor);
    }
    {
        auto newActor = std::make_shared<Car>("Car_02");
        newActor->insertTag("Actor of Car");
        actorsInScene->push_back(newActor);
    }
}
