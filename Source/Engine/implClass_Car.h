#include "Actor.h"
#include "EngineAPI.h"

class Car : public Actor
{
public:
    Car(const std::string& Name) : Actor(Name) { }
    virtual ~Car() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Car";
    }
};
