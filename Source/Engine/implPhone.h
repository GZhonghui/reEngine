#include "Actor.h"

class Phone : public Actor
{
public:
    Phone(const std::string& Name) : Actor(Name) { }
    ~Phone() = default;
public:
    virtual void Init();
    virtual void Update(float Delta);
    virtual void Destroy();
public:
    virtual const char* getType() const noexcept
    {
        return "Phone";
    }
};
