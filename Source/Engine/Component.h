#pragma once

#include"Object.h"

class Component : public Object
{
public:
    Component() = default;
    virtual ~Component() = default;
};