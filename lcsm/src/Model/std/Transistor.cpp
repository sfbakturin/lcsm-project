#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Transistor::Transistor(lcsm::model::Transistor::Type type) : m_type(type)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_base.connectConnect(circuit);
	m_inoutA.connectConnect(circuit);
	m_inoutB.connectConnect(circuit);
}

lcsm::model::Transistor::Transistor(const lcsm::model::Transistor &other) : m_type(other.m_type)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_base.connectConnect(circuit);
	m_inoutA.connectConnect(circuit);
	m_inoutB.connectConnect(circuit);
}

lcsm::model::Transistor::Transistor(lcsm::model::Transistor &&other) noexcept : m_type(other.m_type)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_base.connectConnect(circuit);
	m_inoutA.connectConnect(circuit);
	m_inoutB.connectConnect(circuit);
}

lcsm::model::Transistor &lcsm::model::Transistor::operator=(const lcsm::model::Transistor &other)
{
	return lcsm::support::CopyAssign< lcsm::model::Transistor >(this, other);
}

lcsm::model::Transistor &lcsm::model::Transistor::operator=(lcsm::model::Transistor &&other)
{
	return lcsm::support::MoveAssign< lcsm::model::Transistor >(this, std::move(other));
}

void lcsm::model::Transistor::swap(lcsm::model::Transistor &other) noexcept
{
	std::swap(m_type, other.m_type);
}

lcsm::Identifier lcsm::model::Transistor::idBase() const noexcept
{
	return m_idBase;
}

lcsm::Identifier lcsm::model::Transistor::idInoutA() const noexcept
{
	return m_idInoutA;
}

lcsm::Identifier lcsm::model::Transistor::idInoutB() const noexcept
{
	return m_idInoutB;
}

lcsm::Identifier lcsm::model::Transistor::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Transistor::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	m_idBase = m_id.next();
	m_idInoutA = m_idBase.next();
	m_idInoutB = m_idInoutA.next();

	lcsm::Identifier next = m_base.identify(m_idInoutB.next());
	next = m_inoutA.identify(next);
	next = m_inoutB.identify(next);

	return next;
}

lcsm::ObjectType lcsm::model::Transistor::objectType() const noexcept
{
	return lcsm::ObjectType::PureInt;
}

lcsm::CircuitType lcsm::model::Transistor::circuitType() const noexcept
{
	return lcsm::CircuitType::Transistor;
}

void lcsm::model::Transistor::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Transistor::Port p = static_cast< lcsm::model::Transistor::Port >(portId);
	lcsm::support::PointerView< lcsm::model::Wire > select;
	switch (p)
	{
	case lcsm::model::Transistor::Port::Base:
	{
		select = std::addressof(m_base);
		break;
	}
	case lcsm::model::Transistor::Port::InoutA:
	{
		select = std::addressof(m_inoutA);
		break;
	}
	case lcsm::model::Transistor::Port::InoutB:
	{
		select = std::addressof(m_inoutB);
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
	select->connectToWire(circuit);
}

void lcsm::model::Transistor::connectBase(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Transistor::Port::Base);
	connect(portId, circuit);
}

void lcsm::model::Transistor::connectInoutA(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Transistor::Port::InoutA);
	connect(portId, circuit);
}

void lcsm::model::Transistor::connectInoutB(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Transistor::Port::InoutB);
	connect(portId, circuit);
}
