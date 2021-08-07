#pragma once

class Actor
{
protected:
    virtual void Init() = 0;
    virtual void Update(float Delta) = 0;
    virtual void Destroy() = 0;
public:
    virtual const char* getType() const noexcept = 0;
public:
    friend class EngineCore;
};