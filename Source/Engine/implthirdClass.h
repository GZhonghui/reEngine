#include "Actor.h"
#include "EngineCore.h"

class thirdClass : public Actor
{
public:
    thirdClass(const std::string& Name) : Actor(Name) { }
    virtual ~thirdClass() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "thirdClass";
    }
};
