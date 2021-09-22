#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class B_Glass : public Actor
{
public:
    B_Glass(const std::string& Name) : Actor(Name) { }
    virtual ~B_Glass() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "B_Glass";
    }
};
};
