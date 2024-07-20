#include <sim/Component/CircuitComponent.h>
#include <sim/Component/IOComponent.h>
#include <sim/Component/WiringComponent.h>

const sim::CircuitComponent *sim::CircuitComponent::AsCircuit() const noexcept
{
	return this;
}
sim::CircuitComponent *sim::CircuitComponent::AsCircuit() noexcept
{
	return this;
}

const sim::IOComponent *sim::CircuitComponent::AsIO() const noexcept
{
	return nullptr;
}
sim::IOComponent *sim::CircuitComponent::AsIO() noexcept
{
	return nullptr;
}

const sim::WiringComponent *sim::CircuitComponent::AsWiring() const noexcept
{
	return nullptr;
}
sim::WiringComponent *sim::CircuitComponent::AsWiring() noexcept
{
	return nullptr;
}
