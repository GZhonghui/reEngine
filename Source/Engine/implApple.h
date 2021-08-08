#include "Actor.h"
#include "EngineCore.h"

class Apple : public Actor
{
public:
    Apple(const std::string& Name) : Actor(Name) { }
    virtual ~Apple() = default;
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
