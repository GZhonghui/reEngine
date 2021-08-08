#include "Actor.h"
#include "EngineCore.h"

class Love : public Actor
{
public:
    Love(const std::string& Name) : Actor(Name) { }
    virtual ~Love() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Love";
    }
};
