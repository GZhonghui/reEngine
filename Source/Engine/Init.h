#pragma once

#include<vector>
#include<memory>

#include"Actor.h"

#include"implApple.h"
#include"implCar.h"
#include"implLove.h"

inline void initScene(std::vector<std::shared_ptr<Actor>>* actorsInScene)
{
    actorsInScene->push_back(std::make_shared<Car>("myCar"));
    actorsInScene->push_back(std::make_shared<Apple>("myApple"));
}
