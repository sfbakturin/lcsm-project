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
	m_srcA.connectConnect(circuit);
	m_srcB.connectConnect(circuit);
}

lcsm::model::Transistor::Type lcsm::model::Transistor::type() const noexcept
{
	return m_type;
}

void lcsm::model::Transistor::setType(lcsm::model::Transistor::Type type) noexcept
{
	m_type = type;
}

const lcsm::model::Wire &lcsm::model::Transistor::wireBase() const noexcept
{
	return m_base;
}

const lcsm::model::Wire &lcsm::model::Transistor::wireSrcA() const noexcept
{
	return m_srcA;
}

const lcsm::model::Wire &lcsm::model::Transistor::wireSrcB() const noexcept
{
	return m_srcB;
}

lcsm::Identifier lcsm::model::Transistor::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Transistor::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	lcsm::Identifier next = m_base.identify(m_id.next());
	next = m_srcA.identify(next);
	next = m_srcB.identify(next);
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
		select = std::addressof(m_base);
		break;
	case lcsm::model::Transistor::Port::SrcA:
		select = std::addressof(m_srcA);
		break;
	case lcsm::model::Transistor::Port::SrcB:
		select = std::addressof(m_srcB);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
	select->connectToWire(circuit);
}

void lcsm::model::Transistor::connectBase(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Transistor::Port::Base, circuit);
}

void lcsm::model::Transistor::connectSrcA(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Transistor::Port::SrcA, circuit);
}

void lcsm::model::Transistor::connectSrcB(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Transistor::Port::SrcB, circuit);
}

lcsm::Circuit *lcsm::model::Transistor::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Transistor::Port p = static_cast< lcsm::model::Transistor::Port >(portId);
	switch (p)
	{
	case lcsm::model::Transistor::Port::Base:
		return std::addressof(m_base);
	case lcsm::model::Transistor::Port::SrcA:
		return std::addressof(m_srcA);
	case lcsm::model::Transistor::Port::SrcB:
		return std::addressof(m_srcB);
	}
	return nullptr;
}
