#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/TransmissionGate.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::TransmissionGate::TransmissionGate()
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_base.connectConnect(circuit);
	m_inoutA.connectConnect(circuit);
	m_inoutB.connectConnect(circuit);
	m_inoutC.connectConnect(circuit);
}

lcsm::model::TransmissionGate &lcsm::model::TransmissionGate::operator=(const lcsm::model::TransmissionGate &other)
{
	return lcsm::support::CopyAssign< lcsm::model::TransmissionGate >(this, other);
}

lcsm::model::TransmissionGate &lcsm::model::TransmissionGate::operator=(lcsm::model::TransmissionGate &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::model::TransmissionGate >(this, std::move(other));
}

void lcsm::model::TransmissionGate::swap(lcsm::model::TransmissionGate &) noexcept {}

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

lcsm::Identifier lcsm::model::TransmissionGate::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::TransmissionGate::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
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

lcsm::ObjectType lcsm::model::TransmissionGate::objectType() const noexcept
{
	return lcsm::ObjectType::PureInt;
}

lcsm::CircuitType lcsm::model::TransmissionGate::circuitType() const noexcept
{
	return lcsm::CircuitType::TransmissionGate;
}

void lcsm::model::TransmissionGate::connect(lcsm::portid_t portId, const support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::TransmissionGate::Port p = static_cast< lcsm::model::TransmissionGate::Port >(portId);
	lcsm::support::PointerView< lcsm::model::Wire > selected;
	switch (p)
	{
	case lcsm::model::TransmissionGate::Port::Base:
	{
		selected = std::addressof(m_base);
		break;
	}
	case lcsm::model::TransmissionGate::Port::InoutA:
	{
		selected = std::addressof(m_inoutA);
		break;
	}
	case lcsm::model::TransmissionGate::Port::InoutB:
	{
		selected = std::addressof(m_inoutB);
		break;
	}
	case lcsm::model::TransmissionGate::Port::InoutC:
	{
		selected = std::addressof(m_inoutC);
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
	selected->connectToWire(circuit);
}

void lcsm::model::TransmissionGate::connectBase(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::TransmissionGate::Port::Base);
	connect(portId, circuit);
}

void lcsm::model::TransmissionGate::connectInoutA(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::TransmissionGate::Port::InoutA);
	connect(portId, circuit);
}

void lcsm::model::TransmissionGate::connectInoutB(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::TransmissionGate::Port::InoutB);
	connect(portId, circuit);
}

void lcsm::model::TransmissionGate::connectInoutC(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::TransmissionGate::Port::InoutC);
	connect(portId, circuit);
}
