#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Model/Circuit/TransmissionGate.h>
#include <sim/Model/Wiring/Wire.h>

#include <memory>
#include <stdexcept>
#include <utility>

sim::TransmissionGate::TransmissionGate() :
	m_base(this), m_inoutA(this), m_inoutB(this), m_inoutC(this)
{
}

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
	m_idInoutA = m_idBase.next();
	m_idInoutB = m_idInoutA.next();
	m_idInoutC = m_idInoutB.next();

	sim::Identifier next = m_base.identify(m_idInoutC);
	next = m_inoutA.identify(next);
	next = m_inoutB.identify(next);
	next = m_inoutC.identify(next);

	return next;
}

sim::Identifier sim::TransmissionGate::idBase() const noexcept
{
	return m_idBase;
}

sim::Identifier sim::TransmissionGate::idInoutA() const noexcept
{
	return m_idInoutA;
}

sim::Identifier sim::TransmissionGate::idInoutB() const noexcept
{
	return m_idInoutB;
}

sim::Identifier sim::TransmissionGate::idInoutC() const noexcept
{
	return m_idInoutC;
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

void sim::TransmissionGate::connectInoutA(sim::wire_t &wire)
{
	connectOut(wire, 1);
}

void sim::TransmissionGate::connectInoutB(sim::wire_t &wire)
{
	connectOut(wire, 2);
}

void sim::TransmissionGate::connectInoutC(sim::wire_t &wire)
{
	connectOut(wire, 3);
}

sim::CircuitComponentType sim::TransmissionGate::circuitComponentType() const noexcept
{
	return sim::CircuitComponentType::CIRCUIT_COMP_TRANSMISSION_GATE;
}

void sim::TransmissionGate::connect(sim::wire_t &wire, std::size_t i)
{
	sim::Wire *selected = nullptr;

	switch (i)
	{
	case sim::TransmissionGate::CompositeIndex::BASE:
		selected = std::addressof(m_base);
		break;
	case sim::TransmissionGate::CompositeIndex::INOUT_A:
		selected = std::addressof(m_inoutA);
		break;
	case sim::TransmissionGate::CompositeIndex::INOUT_B:
		selected = std::addressof(m_inoutB);
		break;
	case sim::TransmissionGate::CompositeIndex::INOUT_C:
		selected = std::addressof(m_inoutC);
		break;
	default:
		throw std::logic_error(
			"Transmission gate element has BASE and 3 INOUT "
			"connections.");
	}

	selected->connectWire(wire);
	wire->connectWire(selected);
}
