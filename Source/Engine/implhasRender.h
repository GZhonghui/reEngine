#include "Actor.h"
#include "EngineCore.h"

class hasRender : public Actor
{
public:
    hasRender(const std::string& Name) : Actor(Name) { }
    virtual ~hasRender() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "hasRender";
    }
};
