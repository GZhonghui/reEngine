#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class House : public Actor
{
public:
    House(const std::string& Name) : Actor(Name) { }
    virtual ~House() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "House";
    }
};
};
