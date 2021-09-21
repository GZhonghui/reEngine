#include "Actor.h"
#include "EngineAPI.h"

class Cat : public Actor
{
public:
    Cat(const std::string& Name) : Actor(Name) { }
    virtual ~Cat() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Cat";
    }
};
