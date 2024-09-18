#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Component.h>
#include <sim/Component/IOComponent.h>
#include <sim/Component/WiringComponent.h>

bool sim::Component::isCircuit() const noexcept
{
	return asCircuit() != nullptr;
}

bool sim::Component::isIO() const noexcept
{
	return asIO() != nullptr;
}

bool sim::Component::isWiring() const noexcept
{
	return asWiring() != nullptr;
}

sim::CircuitComponent *sim::Component::asCircuit() noexcept
{
	return nullptr;
}

const sim::CircuitComponent *sim::Component::asCircuit() const noexcept
{
	return nullptr;
}

sim::IOComponent *sim::Component::asIO() noexcept
{
	return nullptr;
}

const sim::IOComponent *sim::Component::asIO() const noexcept
{
	return nullptr;
}

sim::WiringComponent *sim::Component::asWiring() noexcept
{
	return nullptr;
}

const sim::WiringComponent *sim::Component::asWiring() const noexcept
{
	return nullptr;
}
