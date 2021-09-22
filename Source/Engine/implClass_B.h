#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class B : public Actor
{
public:
    B(const std::string& Name) : Actor(Name) { }
    virtual ~B() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "B";
    }
};
};
