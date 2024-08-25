#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Component.h>
#include <sim/Component/IOComponent.h>
#include <sim/Component/WiringComponent.h>

sim::ComponentType sim::CircuitComponent::componentType() const noexcept
{
	return sim::ComponentType::COMP_CIRCUIT;
}

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

bool sim::CircuitComponent::IsPin() const noexcept
{
	return asPin() != nullptr;
}

sim::Pin *sim::CircuitComponent::asPin() noexcept
{
	return nullptr;
}

const sim::Pin *sim::CircuitComponent::asPin() const noexcept
{
	return nullptr;
}

sim::Constant *sim::CircuitComponent::asConstant() noexcept
{
	return nullptr;
}

const sim::Constant *sim::CircuitComponent::asConstant() const noexcept
{
	return nullptr;
}

sim::Power *sim::CircuitComponent::asPower() noexcept
{
	return nullptr;
}

const sim::Power *sim::CircuitComponent::asPower() const noexcept
{
	return nullptr;
}

sim::Ground *sim::CircuitComponent::asGround() noexcept
{
	return nullptr;
}

const sim::Ground *sim::CircuitComponent::asGround() const noexcept
{
	return nullptr;
}
