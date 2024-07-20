#include <sim/Component/Component.h>

bool sim::Component::IsCircuit() const noexcept
{
	return AsCircuit() != nullptr;
}
bool sim::Component::IsIO() const noexcept
{
	return AsIO() != nullptr;
}
bool sim::Component::IsWiring() const noexcept
{
	return AsWiring() != nullptr;
}
