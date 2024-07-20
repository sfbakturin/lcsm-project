#include <sim/Component/Component.h>
#include <sim/Component/WiringComponent.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/Support/Ref.hpp>

#include <stdexcept>
#include <utility>

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
