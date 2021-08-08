#pragma once

#include"ToolAIO.h"
#include"MathAIO.h"

class Actor
{
public:
    Actor(const std::string& Name) :m_Name(Name) {}
    virtual ~Actor() = default;
protected:
    std::string m_Name;
public:
    std::string getName() const noexcept { return m_Name; }
public:
    virtual void Init() = 0;
    virtual void Update(float Delta) = 0;
    virtual void Destroy() = 0;
public:
    virtual const char* getType() const noexcept = 0;
};