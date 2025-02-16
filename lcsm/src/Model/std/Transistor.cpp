#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Transistor::Transistor(lcsm::model::Transistor::Type type) : m_type(type) {}

lcsm::model::Transistor::Type lcsm::model::Transistor::type() const noexcept
{
	return m_type;
}

void lcsm::model::Transistor::setType(lcsm::model::Transistor::Type type) noexcept
{
	m_type = type;
}

const lcsm::model::Wire *lcsm::model::Transistor::wireBase() const noexcept
{
	return m_base.get();
}

const lcsm::model::Wire *lcsm::model::Transistor::wireSrcA() const noexcept
{
	return m_srcA.get();
}

const lcsm::model::Wire *lcsm::model::Transistor::wireSrcB() const noexcept
{
	return m_srcB.get();
}

std::size_t lcsm::model::Transistor::numOfWires() const noexcept
{
	return 3;
}

void lcsm::model::Transistor::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_base = wires[0];
	m_srcA = wires[1];
	m_srcB = wires[2];
	m_base->connectConnect(this);
	m_srcA->connectConnect(this);
	m_srcB->connectConnect(this);
}

lcsm::Identifier lcsm::model::Transistor::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Transistor::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	lcsm::Identifier next = m_base->identify(m_id.next());
	next = m_srcA->identify(next);
	next = m_srcB->identify(next);
	return next;
}

lcsm::object_type_t lcsm::model::Transistor::objectType() const noexcept
{
	return lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Transistor::circuitType() const noexcept
{
	return lcsm::CircuitType::Transistor;
}

void lcsm::model::Transistor::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Transistor::disconnect(lcsm::Circuit *)
{
	// Do nothing.
}

void lcsm::model::Transistor::disconnectAll()
{
	m_base->disconnectAll();
	m_srcA->disconnectAll();
	m_srcB->disconnectAll();
}

void lcsm::model::Transistor::connectBase(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Transistor::Port::Base, circuit);
}

void lcsm::model::Transistor::connectSrcA(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Transistor::Port::SrcA, circuit);
}

void lcsm::model::Transistor::connectSrcB(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Transistor::Port::SrcB, circuit);
}

lcsm::Circuit *lcsm::model::Transistor::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Transistor::Port p = static_cast< lcsm::model::Transistor::Port >(portId);
	switch (p)
	{
	case lcsm::model::Transistor::Port::Base:
		return m_base.get();
	case lcsm::model::Transistor::Port::SrcA:
		return m_srcA.get();
	case lcsm::model::Transistor::Port::SrcB:
		return m_srcB.get();
	}
	return nullptr;
}
