#include "Actor.h"
#include "EngineCore.h"

class Bee : public Actor
{
public:
    Bee(const std::string& Name) : Actor(Name) { }
    virtual ~Bee() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Bee";
    }
};
