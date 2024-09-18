#include <sim/Component/Identifier.h>
#include <sim/Model/IO/Probe.h>
#include <sim/Model/Wiring/Wire.h>

#include <stdexcept>
#include <utility>

sim::Identifier sim::Probe::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Probe::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wireIn.identify(m_id.next());
}

void sim::Probe::connectIn(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Probe element has only 1 input.");
	m_wireIn.connectWire(wire);
}
void sim::Probe::connectOut(sim::wire_t &, std::size_t)
{
	throw std::logic_error("Probe element doesn't have any outputs.");
}
