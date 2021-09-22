#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class B_Cartoon : public Actor
{
public:
    B_Cartoon(const std::string& Name) : Actor(Name) { }
    virtual ~B_Cartoon() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "B_Cartoon";
    }
};
};
