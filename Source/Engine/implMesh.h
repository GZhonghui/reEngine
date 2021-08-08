#include "Actor.h"
#include "EngineCore.h"

class Mesh : public Actor
{
public:
    Mesh(const std::string& Name) : Actor(Name) { }
    ~Mesh() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Mesh";
    }
};
