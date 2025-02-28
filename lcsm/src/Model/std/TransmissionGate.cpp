#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/TransmissionGate.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::TransmissionGate::TransmissionGate(lcsm::label_t name) : lcsm::Circuit(name) {}

lcsm::model::TransmissionGate::~TransmissionGate() noexcept
{
	disconnectAll();
}

const lcsm::model::Wire *lcsm::model::TransmissionGate::wireBase() const noexcept
{
	return m_base.get();
}

const lcsm::model::Wire *lcsm::model::TransmissionGate::wireSrcA() const noexcept
{
	return m_srcA.get();
}

const lcsm::model::Wire *lcsm::model::TransmissionGate::wireSrcB() const noexcept
{
	return m_srcB.get();
}

const lcsm::model::Wire *lcsm::model::TransmissionGate::wireSrcC() const noexcept
{
	return m_srcC.get();
}

std::size_t lcsm::model::TransmissionGate::numOfWires() const noexcept
{
	return 4;
}

void lcsm::model::TransmissionGate::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_base = wires[0];
	m_srcA = wires[1];
	m_srcB = wires[2];
	m_srcC = wires[3];
	m_base->connectConnect(this);
	m_srcA->connectConnect(this);
	m_srcB->connectConnect(this);
	m_srcC->connectConnect(this);
}

lcsm::Identifier lcsm::model::TransmissionGate::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::TransmissionGate::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	lcsm::Identifier next = m_base->identify(m_id);
	next = m_srcA->identify(next);
	next = m_srcB->identify(next);
	next = m_srcC->identify(next);
	return next;
}

lcsm::object_type_t lcsm::model::TransmissionGate::objectType() const noexcept
{
	return lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::TransmissionGate::circuitType() const noexcept
{
	return lcsm::CircuitType::TransmissionGate;
}

void lcsm::model::TransmissionGate::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::TransmissionGate::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::TransmissionGate::disconnectAll() noexcept
{
	m_base->disconnectAll();
	m_srcA->disconnectAll();
	m_srcB->disconnectAll();
	m_srcC->disconnectAll();
}

void lcsm::model::TransmissionGate::connectBase(lcsm::Circuit *circuit)
{
	connect(lcsm::model::TransmissionGate::Port::Base, circuit);
}

void lcsm::model::TransmissionGate::connectSrcA(lcsm::Circuit *circuit)
{
	connect(lcsm::model::TransmissionGate::Port::SrcA, circuit);
}

void lcsm::model::TransmissionGate::connectSrcB(lcsm::Circuit *circuit)
{
	connect(lcsm::model::TransmissionGate::Port::SrcB, circuit);
}

void lcsm::model::TransmissionGate::connectSrcC(lcsm::Circuit *circuit)
{
	connect(lcsm::model::TransmissionGate::Port::SrcC, circuit);
}

lcsm::Circuit *lcsm::model::TransmissionGate::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::TransmissionGate::Port p = static_cast< lcsm::model::TransmissionGate::Port >(portId);
	switch (p)
	{
	case lcsm::model::TransmissionGate::Port::Base:
		return m_base.get();
	case lcsm::model::TransmissionGate::Port::SrcA:
		return m_srcA.get();
	case lcsm::model::TransmissionGate::Port::SrcB:
		return m_srcB.get();
	case lcsm::model::TransmissionGate::Port::SrcC:
		return m_srcC.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::TransmissionGate::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_base.get())
		return lcsm::model::TransmissionGate::Port::Base;
	else if (circuit == m_srcA.get())
		return lcsm::model::TransmissionGate::Port::SrcA;
	else if (circuit == m_srcB.get())
		return lcsm::model::TransmissionGate::Port::SrcB;
	else if (circuit == m_srcC.get())
		return lcsm::model::TransmissionGate::Port::SrcC;
	else
		return -1;
}

lcsm::portid_t lcsm::model::TransmissionGate::defaultPort() const noexcept
{
	return lcsm::model::TransmissionGate::Port::Base;
}
