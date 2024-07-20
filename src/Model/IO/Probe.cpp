#include <sim/Model/IO/Probe.h>
#include <sim/Model/Wiring/Wire.h>

#include <stdexcept>

void sim::Probe::ConnectIn(const sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Probe element has only 1 input.");
	m_wireIn.Val().ConnectWire(wire);
}
void sim::Probe::ConnectOut(const sim::wire_t &wire, std::size_t i)
{
	throw std::logic_error("Probe element doesn't have any outputs.");
}
