#include <sim/Component/CircuitComponent.h>
#include <sim/Component/WiringComponent.h>
#include <sim/Model/Wiring/Tunnel.h>
#include <sim/Model/Wiring/Wire.h>

sim::ComponentType sim::WiringComponent::componentType() const noexcept
{
	return sim::ComponentType::COMP_WIRING;
}

sim::WiringComponent *sim::WiringComponent::asWiring() noexcept
{
	return this;
}

const sim::WiringComponent *sim::WiringComponent::asWiring() const noexcept
{
	return this;
}

bool sim::WiringComponent::isWire() const noexcept
{
	return wiringComponentType() == sim::WiringComponentType::WIRING_COMP_WIRE;
}

bool sim::WiringComponent::isTunnel() const noexcept
{
	return wiringComponentType() == sim::WiringComponentType::WIRING_COMP_TUNNEL;
}

sim::Wire *sim::WiringComponent::asWire() noexcept
{
	return nullptr;
}

const sim::Wire *sim::WiringComponent::asWire() const noexcept
{
	return nullptr;
}

sim::Tunnel *sim::WiringComponent::asTunnel() noexcept
{
	return nullptr;
}

const sim::Tunnel *sim::WiringComponent::asTunnel() const noexcept
{
	return nullptr;
}
