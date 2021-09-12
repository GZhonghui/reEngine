#include "Actor.h"
#include "EngineCore.h"

class noRender : public Actor
{
public:
    noRender(const std::string& Name) : Actor(Name) { }
    virtual ~noRender() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "noRender";
    }
};
