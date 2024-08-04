#include <sim/IR/CGObject.h>

bool sim::CGObject::IsPinInput() const noexcept
{
	return T() == sim::CGObjectT::OBJ_PIN_INPUT;
}

bool sim::CGObject::IsPinOutput() const noexcept
{
	return T() == sim::CGObjectT::OBJ_PIN_OUTPUT;
}

bool sim::CGObject::IsWire() const noexcept
{
	return T() == sim::CGObjectT::OBJ_WIRE;
}
