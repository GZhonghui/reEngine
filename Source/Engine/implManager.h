#include "Actor.h"
#include "EngineCore.h"

class Manager : public Actor
{
public:
    Manager(const std::string& Name) : Actor(Name) { }
    virtual ~Manager() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Manager";
    }
};
