#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Component.h>

sim::ComponentType sim::CircuitComponent::componentType() const noexcept
{
	return sim::ComponentType::COMP_CIRCUIT;
}

sim::CircuitComponent *sim::CircuitComponent::asCircuit() noexcept
{
	return this;
}

const sim::CircuitComponent *sim::CircuitComponent::asCircuit() const noexcept
{
	return this;
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

sim::Transistor *sim::CircuitComponent::asTransistor() noexcept
{
	return nullptr;
}

const sim::Transistor *sim::CircuitComponent::asTransistor() const noexcept
{
	return nullptr;
}
