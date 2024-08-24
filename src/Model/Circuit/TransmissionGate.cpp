#include "sim/Component/CircuitComponent.h"
#include <sim/Model/Circuit/TransmissionGate.h>
#include <sim/Model/Wiring/Wire.h>

#include <stdexcept>

sim::TransmissionGate::TransmissionGate() : m_id(0) {}

sim::TransmissionGate::TransmissionGate(const sim::TransmissionGate &) {}

sim::TransmissionGate::TransmissionGate(sim::TransmissionGate &&) noexcept {}

unsigned sim::TransmissionGate::ID() const noexcept
{
	return m_id;
}

void sim::TransmissionGate::Identify(unsigned ID) noexcept
{
	m_id = ID;
}

void sim::TransmissionGate::ConnectIn(sim::wire_t &wire, std::size_t i)
{
	Connect(wire, i);
}

void sim::TransmissionGate::ConnectOut(sim::wire_t &wire, std::size_t i)
{
	Connect(wire, i);
}

void sim::TransmissionGate::ConnectBase(sim::wire_t &wire)
{
	ConnectIn(wire, 0);
}

void sim::TransmissionGate::ConnectSrcA(sim::wire_t &wire)
{
	ConnectOut(wire, 1);
}

void sim::TransmissionGate::ConnectSrcB(sim::wire_t &wire)
{
	ConnectOut(wire, 2);
}

void sim::TransmissionGate::ConnectSrcC(sim::wire_t &wire)
{
	ConnectOut(wire, 3);
}

sim::CircuitComponentType sim::TransmissionGate::circuitComponentType() const noexcept
{
	return sim::CircuitComponentType::CIRCUIT_COMP_TRANSMISSION_GATE;
}

const sim::Pin *sim::TransmissionGate::AsPin() const noexcept
{
	return nullptr;
}

sim::Pin *sim::TransmissionGate::AsPin() noexcept
{
	return nullptr;
}

void sim::TransmissionGate::Connect(sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
		m_base.ConnectWire(wire);
	else if (i - 1 < sim::TransmissionGate::SRC_N)
		m_srcs[i].ConnectWire(wire);
	throw std::logic_error("TransmissionGate element has only 4 inout "
						   "connections.");
}
