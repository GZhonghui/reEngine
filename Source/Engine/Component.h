#pragma once

#include"Object.h"

class Component : public Object
{
public:
    Component() = default;
    virtual ~Component() = default;
public:
    virtual void Init() = 0;
    virtual void Update(float Delta) = 0;
    virtual void Destroy() = 0;
};