#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Component.h>
#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/WiringComponent.h>

bool lcsm::Component::isCircuit() const noexcept
{
	return asCircuit() != nullptr;
}

bool lcsm::Component::isIO() const noexcept
{
	return asIO() != nullptr;
}

bool lcsm::Component::isWiring() const noexcept
{
	return asWiring() != nullptr;
}

lcsm::CircuitComponent *lcsm::Component::asCircuit() noexcept
{
	return nullptr;
}

const lcsm::CircuitComponent *lcsm::Component::asCircuit() const noexcept
{
	return nullptr;
}

lcsm::IOComponent *lcsm::Component::asIO() noexcept
{
	return nullptr;
}

const lcsm::IOComponent *lcsm::Component::asIO() const noexcept
{
	return nullptr;
}

lcsm::WiringComponent *lcsm::Component::asWiring() noexcept
{
	return nullptr;
}

const lcsm::WiringComponent *lcsm::Component::asWiring() const noexcept
{
	return nullptr;
}
