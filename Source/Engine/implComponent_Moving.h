#include "Component.h"
#include "EngineAPI.h"

namespace UserComponent
{
class Moving : public Component
{
public:
    Moving(const std::string& Owner) : Component(Owner) { }
    virtual ~Moving() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
};
};
