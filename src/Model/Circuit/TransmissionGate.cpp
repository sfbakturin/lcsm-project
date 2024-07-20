#include <sim/Model/Circuit/TransmissionGate.h>
#include <sim/Model/Wiring/Wire.h>

#include <stdexcept>

sim::TransmissionGate::TransmissionGate() noexcept {}

sim::TransmissionGate::TransmissionGate(const sim::TransmissionGate &other) noexcept {}
sim::TransmissionGate::TransmissionGate(sim::TransmissionGate &&other) noexcept {}

void sim::TransmissionGate::ConnectIn(const sim::wire_t &wire, std::size_t i)
{
	Connect(wire, i);
}
void sim::TransmissionGate::ConnectOut(const sim::wire_t &wire, std::size_t i)
{
	Connect(wire, i);
}

void sim::TransmissionGate::ConnectBase(const sim::wire_t &wire)
{
	ConnectIn(wire, 0);
}
void sim::TransmissionGate::ConnectSrcA(const sim::wire_t &wire)
{
	ConnectOut(wire, 1);
}
void sim::TransmissionGate::ConnectSrcB(const sim::wire_t &wire)
{
	ConnectOut(wire, 2);
}
void sim::TransmissionGate::ConnectSrcC(const sim::wire_t &wire)
{
	ConnectOut(wire, 3);
}

void sim::TransmissionGate::Connect(const sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
		m_base.Val().ConnectWire(wire);
	else if (i - 1 < sim::TransmissionGate::SRC_N)
		m_srcs[i].Val().ConnectWire(wire);
	throw std::logic_error("TransmissionGate element has only 4 inout connections.");
}
