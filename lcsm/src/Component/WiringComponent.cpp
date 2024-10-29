#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/Model/Wiring/Tunnel.h>
#include <lcsm/Model/Wiring/Wire.h>

lcsm::ComponentType lcsm::WiringComponent::componentType() const noexcept
{
	return lcsm::ComponentType::COMP_WIRING;
}

lcsm::WiringComponent *lcsm::WiringComponent::asWiring() noexcept
{
	return this;
}

const lcsm::WiringComponent *lcsm::WiringComponent::asWiring() const noexcept
{
	return this;
}

bool lcsm::WiringComponent::isWire() const noexcept
{
	return wiringComponentType() == lcsm::WiringComponentType::WIRING_COMP_WIRE;
}

bool lcsm::WiringComponent::isTunnel() const noexcept
{
	return wiringComponentType() == lcsm::WiringComponentType::WIRING_COMP_TUNNEL;
}

lcsm::model::Wire *lcsm::WiringComponent::asWire() noexcept
{
	return nullptr;
}

const lcsm::model::Wire *lcsm::WiringComponent::asWire() const noexcept
{
	return nullptr;
}

lcsm::model::Tunnel *lcsm::WiringComponent::asTunnel() noexcept
{
	return nullptr;
}

const lcsm::model::Tunnel *lcsm::WiringComponent::asTunnel() const noexcept
{
	return nullptr;
}
