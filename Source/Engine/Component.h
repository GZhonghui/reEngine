#pragma once

#include"Object.h"

class Component : public Object
{
public:
    Component(const std::string& Owner) :m_Owner(Owner) {}
    virtual ~Component() = default;
protected:
    std::string m_Owner;
public:
    virtual void Init() = 0;
    virtual void Update(float Delta) = 0;
    virtual void Destroy() = 0;
};