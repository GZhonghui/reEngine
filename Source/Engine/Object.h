#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

class Object
{
public:
    Object() = default;
    virtual ~Object() = default;
public:
    virtual void Init() = 0;
    virtual void Update(float Delta) = 0;
    virtual void Destroy() = 0;
};