#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class B_Normal : public Actor
{
public:
    B_Normal(const std::string& Name) : Actor(Name) { }
    virtual ~B_Normal() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "B_Normal";
    }
};
};
