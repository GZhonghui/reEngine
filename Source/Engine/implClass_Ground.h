#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class Ground : public Actor
{
public:
    Ground(const std::string& Name) : Actor(Name) { }
    virtual ~Ground() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Ground";
    }
};
};
