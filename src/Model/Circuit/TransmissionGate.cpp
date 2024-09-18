#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Model/Circuit/TransmissionGate.h>
#include <sim/Model/Wiring/Wire.h>

#include <stdexcept>
#include <utility>

sim::TransmissionGate::TransmissionGate(const sim::TransmissionGate &) {}

sim::TransmissionGate::TransmissionGate(sim::TransmissionGate &&) noexcept {}

sim::Identifier sim::TransmissionGate::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::TransmissionGate::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	m_idBase = m_id.next();
	m_idSrcA = m_idBase.next();
	m_idSrcB = m_idSrcA.next();
	m_idSrcC = m_idSrcB.next();
	sim::Identifier next = m_base.identify(m_idSrcC.next());
	for (std::size_t i = 0; i < sim::TransmissionGate::SRC_N; i++)
		next = m_srcs[i].identify(next.next());
	return next;
}

sim::Identifier sim::TransmissionGate::idBase() const noexcept
{
	return m_idBase;
}

sim::Identifier sim::TransmissionGate::idSrcA() const noexcept
{
	return m_idSrcA;
}

sim::Identifier sim::TransmissionGate::idSrcB() const noexcept
{
	return m_idSrcB;
}

sim::Identifier sim::TransmissionGate::idSrcC() const noexcept
{
	return m_idSrcC;
}

void sim::TransmissionGate::connectIn(sim::wire_t &wire, std::size_t i)
{
	connect(wire, i);
}

void sim::TransmissionGate::connectOut(sim::wire_t &wire, std::size_t i)
{
	connect(wire, i);
}

void sim::TransmissionGate::connectBase(sim::wire_t &wire)
{
	connectIn(wire, 0);
}

void sim::TransmissionGate::connectSrcA(sim::wire_t &wire)
{
	connectOut(wire, 1);
}

void sim::TransmissionGate::connectSrcB(sim::wire_t &wire)
{
	connectOut(wire, 2);
}

void sim::TransmissionGate::connectSrcC(sim::wire_t &wire)
{
	connectOut(wire, 3);
}

sim::CircuitComponentType sim::TransmissionGate::circuitComponentType() const noexcept
{
	return sim::CircuitComponentType::CIRCUIT_COMP_TRANSMISSION_GATE;
}

void sim::TransmissionGate::connect(sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
		m_base.connectWire(wire);
	else if (i - 1 < sim::TransmissionGate::SRC_N)
		m_srcs[i].connectWire(wire);
	throw std::logic_error(
		"TransmissionGate element has only 4 inout "
		"connections.");
}
