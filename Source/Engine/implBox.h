#include "Actor.h"
#include "EngineAPI.h"

class Box : public Actor
{
public:
    Box(const std::string& Name) : Actor(Name) { }
    virtual ~Box() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Box";
    }
};
