#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Circuit/TransmissionGate.h>
#include <lcsm/Model/Wiring/Wire.h>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::TransmissionGate::TransmissionGate() : m_base(this), m_inoutA(this), m_inoutB(this), m_inoutC(this) {}

lcsm::model::TransmissionGate::TransmissionGate(const lcsm::model::TransmissionGate &) {}

lcsm::model::TransmissionGate::TransmissionGate(lcsm::model::TransmissionGate &&) noexcept {}

lcsm::Identifier lcsm::model::TransmissionGate::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::TransmissionGate::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	m_idBase = m_id.next();
	m_idInoutA = m_idBase.next();
	m_idInoutB = m_idInoutA.next();
	m_idInoutC = m_idInoutB.next();

	lcsm::Identifier next = m_base.identify(m_idInoutC);
	next = m_inoutA.identify(next);
	next = m_inoutB.identify(next);
	next = m_inoutC.identify(next);

	return next;
}

lcsm::Identifier lcsm::model::TransmissionGate::idBase() const noexcept
{
	return m_idBase;
}

lcsm::Identifier lcsm::model::TransmissionGate::idInoutA() const noexcept
{
	return m_idInoutA;
}

lcsm::Identifier lcsm::model::TransmissionGate::idInoutB() const noexcept
{
	return m_idInoutB;
}

lcsm::Identifier lcsm::model::TransmissionGate::idInoutC() const noexcept
{
	return m_idInoutC;
}

void lcsm::model::TransmissionGate::connectIn(lcsm::wire_t &wire, std::size_t i)
{
	connect(wire, i);
}

void lcsm::model::TransmissionGate::connectOut(lcsm::wire_t &wire, std::size_t i)
{
	connect(wire, i);
}

void lcsm::model::TransmissionGate::connectBase(lcsm::wire_t &wire)
{
	connectIn(wire, 0);
}

void lcsm::model::TransmissionGate::connectInoutA(lcsm::wire_t &wire)
{
	connectOut(wire, 1);
}

void lcsm::model::TransmissionGate::connectInoutB(lcsm::wire_t &wire)
{
	connectOut(wire, 2);
}

void lcsm::model::TransmissionGate::connectInoutC(lcsm::wire_t &wire)
{
	connectOut(wire, 3);
}

lcsm::CircuitComponentType lcsm::model::TransmissionGate::circuitComponentType() const noexcept
{
	return lcsm::CircuitComponentType::CIRCUIT_COMP_TRANSMISSION_GATE;
}

void lcsm::model::TransmissionGate::connect(lcsm::wire_t &wire, std::size_t i)
{
	lcsm::model::Wire *selected = nullptr;

	switch (i)
	{
	case lcsm::model::TransmissionGate::CompositeIndex::BASE:
		selected = std::addressof(m_base);
		break;
	case lcsm::model::TransmissionGate::CompositeIndex::INOUT_A:
		selected = std::addressof(m_inoutA);
		break;
	case lcsm::model::TransmissionGate::CompositeIndex::INOUT_B:
		selected = std::addressof(m_inoutB);
		break;
	case lcsm::model::TransmissionGate::CompositeIndex::INOUT_C:
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
