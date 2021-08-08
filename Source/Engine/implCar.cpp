#include "implCar.h"

void Car::Init()
{
	Out::Log(pType::MESSAGE, "Creating A Car!");
}

void Car::Update(float Delta)
{
	Out::Log(pType::MESSAGE, "Updating Car! (%10.4f)", Delta);
}

void Car::Destroy()
{

}
