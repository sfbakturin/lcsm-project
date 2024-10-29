#include <lcsm/Component/Component.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/Model/Wiring/Wire.h>
#include <lcsm/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>

lcsm::model::Wire::Wire(lcsm::Component *comp)
{
	connect(lcsm::support::PointerView< lcsm::Component >(comp));
}

lcsm::Identifier lcsm::model::Wire::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Wire::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_id.next();
}

void lcsm::model::Wire::connectIn(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("");
	connectWire(wire);
}

void lcsm::model::Wire::connectOut(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("");
	connectWire(wire);
}

void lcsm::model::Wire::connectWire(const lcsm::wire_t &wire)
{
	m_wires.push_back(wire);
}

void lcsm::model::Wire::connectWire(lcsm::wire_t &&wire)
{
	m_wires.push_back(std::move(wire));
}

void lcsm::model::Wire::connect(const lcsm::component_t &connect)
{
	if (connect->isWiring())
		throw std::logic_error(
			"All wiring components should be connected via "
			"ConnectWire method.");
	m_connections.push_back(connect);
}

void lcsm::model::Wire::connect(lcsm::component_t &&connect)
{
	if (connect->isWiring())
		throw std::logic_error(
			"All wiring components should be connected via "
			"ConnectWire method.");
	m_connections.push_back(std::move(connect));
}

const std::vector< lcsm::wire_t > &lcsm::model::Wire::wires() const noexcept
{
	return m_wires;
}

const std::vector< lcsm::component_t > &lcsm::model::Wire::connections() const noexcept
{
	return m_connections;
}

lcsm::WiringComponentType lcsm::model::Wire::wiringComponentType() const noexcept
{
	return lcsm::WiringComponentType::WIRING_COMP_WIRE;
}

lcsm::model::Wire *lcsm::model::Wire::asWire() noexcept
{
	return this;
}

const lcsm::model::Wire *lcsm::model::Wire::asWire() const noexcept
{
	return this;
}
