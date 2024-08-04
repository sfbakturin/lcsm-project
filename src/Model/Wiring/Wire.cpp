#include <sim/Component/Component.h>
#include <sim/Component/WiringComponent.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/Support/Ref.hpp>

#include <stdexcept>
#include <utility>

sim::Wire::Wire() : m_id(0) {}

sim::Wire::Wire(sim::Component *comp) : m_id(0)
{
	Connect(sim::support::Ref< sim::Component >(comp));
}

unsigned sim::Wire::ID() const noexcept
{
	return m_id;
}

void sim::Wire::Identify(unsigned ID) noexcept
{
	m_id = ID;
}

void sim::Wire::ConnectIn(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("");
	ConnectWire(wire);
}

void sim::Wire::ConnectOut(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("");
	ConnectWire(wire);
}

void sim::Wire::ConnectWire(const sim::wire_t &wire)
{
	m_wires.push_back(wire);
}

void sim::Wire::ConnectWire(sim::wire_t &&wire)
{
	m_wires.push_back(std::move(wire));
}

void sim::Wire::Connect(const sim::component_t &connect)
{
	if (connect.Val().IsWiring())
		throw std::logic_error("All wiring components should be connected via ConnectWire method.");
	m_connections.push_back(connect);
}

void sim::Wire::Connect(sim::component_t &&connect)
{
	if (connect.Val().IsWiring())
		throw std::logic_error("All wiring components should be connected via ConnectWire method.");
	m_connections.push_back(std::move(connect));
}

const std::vector< sim::wire_t > &sim::Wire::Wires() const noexcept
{
	return m_wires;
}

const std::vector< sim::component_t > &sim::Wire::Connections() const noexcept
{
	return m_connections;
}

const sim::Wire *sim::Wire::AsWire() const noexcept
{
	return this;
}

sim::Wire *sim::Wire::AsWire() noexcept
{
	return this;
}
