#include "Actor.h"
#include "EngineAPI.h"

class Dog : public Actor
{
public:
    Dog(const std::string& Name) : Actor(Name) { }
    virtual ~Dog() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Dog";
    }
};
