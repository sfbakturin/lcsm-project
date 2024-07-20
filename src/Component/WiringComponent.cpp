#include <sim/Component/CircuitComponent.h>
#include <sim/Component/IOComponent.h>
#include <sim/Component/WiringComponent.h>

const sim::CircuitComponent *sim::WiringComponent::AsCircuit() const noexcept
{
	return nullptr;
}
sim::CircuitComponent *sim::WiringComponent::AsCircuit() noexcept
{
	return nullptr;
}

const sim::IOComponent *sim::WiringComponent::AsIO() const noexcept
{
	return nullptr;
}
sim::IOComponent *sim::WiringComponent::AsIO() noexcept
{
	return nullptr;
}

const sim::WiringComponent *sim::WiringComponent::AsWiring() const noexcept
{
	return this;
}
sim::WiringComponent *sim::WiringComponent::AsWiring() noexcept
{
	return this;
}
