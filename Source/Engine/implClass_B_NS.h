#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class B_NS : public Actor
{
public:
    B_NS(const std::string& Name) : Actor(Name) { }
    virtual ~B_NS() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "B_NS";
    }
};
};
