#include "Actor.h"

class Apple : public Actor
{
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Apple";
    }
};
