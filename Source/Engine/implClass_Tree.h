#include "Actor.h"
#include "EngineAPI.h"

namespace UserClass
{
class Tree : public Actor
{
public:
    Tree(const std::string& Name) : Actor(Name) { }
    virtual ~Tree() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Tree";
    }
};
};
