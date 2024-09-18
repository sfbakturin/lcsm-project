#include <sim/Component/Component.h>
#include <sim/Component/Identifier.h>
#include <sim/Component/WiringComponent.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>

sim::Wire::Wire(sim::Component *comp)
{
	connect(sim::support::PointerView< sim::Component >(comp));
}

sim::Identifier sim::Wire::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Wire::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_id.next();
}

void sim::Wire::connectIn(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("");
	connectWire(wire);
}

void sim::Wire::connectOut(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("");
	connectWire(wire);
}

void sim::Wire::connectWire(const sim::wire_t &wire)
{
	m_wires.push_back(wire);
}

void sim::Wire::connectWire(sim::wire_t &&wire)
{
	m_wires.push_back(std::move(wire));
}

void sim::Wire::connect(const sim::component_t &connect)
{
	if (connect->isWiring())
		throw std::logic_error(
			"All wiring components should be connected via "
			"ConnectWire method.");
	m_connections.push_back(connect);
}

void sim::Wire::connect(sim::component_t &&connect)
{
	if (connect->isWiring())
		throw std::logic_error(
			"All wiring components should be connected via "
			"ConnectWire method.");
	m_connections.push_back(std::move(connect));
}

const std::vector< sim::wire_t > &sim::Wire::wires() const noexcept
{
	return m_wires;
}

const std::vector< sim::component_t > &sim::Wire::connections() const noexcept
{
	return m_connections;
}

sim::WiringComponentType sim::Wire::wiringComponentType() const noexcept
{
	return sim::WiringComponentType::WIRING_COMP_WIRE;
}

sim::Wire *sim::Wire::asWire() noexcept
{
	return this;
}

const sim::Wire *sim::Wire::asWire() const noexcept
{
	return this;
}
