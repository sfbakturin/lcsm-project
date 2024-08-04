#include <sim/Model/IO/Probe.h>
#include <sim/Model/Wiring/Wire.h>

#include <stdexcept>

sim::Probe::Probe() : m_id(0) {}

unsigned sim::Probe::ID() const noexcept
{
	return m_id;
}

void sim::Probe::Identify(unsigned ID) noexcept
{
	m_id = ID;
}

void sim::Probe::ConnectIn(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Probe element has only 1 input.");
	m_wireIn.ConnectWire(wire);
}
void sim::Probe::ConnectOut(sim::wire_t &, std::size_t)
{
	throw std::logic_error("Probe element doesn't have any outputs.");
}
