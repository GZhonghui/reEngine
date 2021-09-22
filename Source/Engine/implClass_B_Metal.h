#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class B_Metal : public Actor
{
public:
    B_Metal(const std::string& Name) : Actor(Name) { }
    virtual ~B_Metal() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "B_Metal";
    }
};
};
