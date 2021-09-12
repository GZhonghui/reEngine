#include "Actor.h"
#include "EngineCore.h"

class BlueBoy : public Actor
{
public:
    BlueBoy(const std::string& Name) : Actor(Name) { }
    virtual ~BlueBoy() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "BlueBoy";
    }
};
