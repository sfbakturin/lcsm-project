#include <sim/Component/CircuitComponent.h>
#include <sim/Component/IOComponent.h>
#include <sim/Component/WiringComponent.h>

const sim::CircuitComponent *sim::IOComponent::AsCircuit() const noexcept
{
	return nullptr;
}
sim::CircuitComponent *sim::IOComponent::AsCircuit() noexcept
{
	return nullptr;
}

const sim::IOComponent *sim::IOComponent::AsIO() const noexcept
{
	return this;
}
sim::IOComponent *sim::IOComponent::AsIO() noexcept
{
	return this;
}

const sim::WiringComponent *sim::IOComponent::AsWiring() const noexcept
{
	return nullptr;
}
sim::WiringComponent *sim::IOComponent::AsWiring() noexcept
{
	return nullptr;
}
