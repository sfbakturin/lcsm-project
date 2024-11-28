#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Component.h>

lcsm::ComponentType lcsm::CircuitComponent::componentType() const noexcept
{
	return lcsm::ComponentType::COMP_CIRCUIT;
}

lcsm::CircuitComponent *lcsm::CircuitComponent::asCircuit() noexcept
{
	return this;
}

const lcsm::CircuitComponent *lcsm::CircuitComponent::asCircuit() const noexcept
{
	return this;
}

lcsm::model::Pin *lcsm::CircuitComponent::asPin() noexcept
{
	return nullptr;
}

const lcsm::model::Pin *lcsm::CircuitComponent::asPin() const noexcept
{
	return nullptr;
}

lcsm::model::Constant *lcsm::CircuitComponent::asConstant() noexcept
{
	return nullptr;
}

const lcsm::model::Constant *lcsm::CircuitComponent::asConstant() const noexcept
{
	return nullptr;
}

lcsm::model::Power *lcsm::CircuitComponent::asPower() noexcept
{
	return nullptr;
}

const lcsm::model::Power *lcsm::CircuitComponent::asPower() const noexcept
{
	return nullptr;
}

lcsm::model::Ground *lcsm::CircuitComponent::asGround() noexcept
{
	return nullptr;
}

const lcsm::model::Ground *lcsm::CircuitComponent::asGround() const noexcept
{
	return nullptr;
}

lcsm::model::Splitter *lcsm::CircuitComponent::asSplitter() noexcept
{
	return nullptr;
}

const lcsm::model::Splitter *lcsm::CircuitComponent::asSplitter() const noexcept
{
	return nullptr;
}

lcsm::model::Clock *lcsm::CircuitComponent::asClock() noexcept
{
	return nullptr;
}

const lcsm::model::Clock *lcsm::CircuitComponent::asClock() const noexcept
{
	return nullptr;
}

lcsm::model::Transistor *lcsm::CircuitComponent::asTransistor() noexcept
{
	return nullptr;
}

const lcsm::model::Transistor *lcsm::CircuitComponent::asTransistor() const noexcept
{
	return nullptr;
}

lcsm::model::TransmissionGate *lcsm::CircuitComponent::asTransmissionGate() noexcept
{
	return nullptr;
}

const lcsm::model::TransmissionGate *lcsm::CircuitComponent::asTransmissionGate() const noexcept
{
	return nullptr;
}
