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
	m_srcA.connectConnect(circuit);
	m_srcB.connectConnect(circuit);
	m_srcC.connectConnect(circuit);
}

const lcsm::model::Wire &lcsm::model::TransmissionGate::wireBase() const noexcept
{
	return m_base;
}

const lcsm::model::Wire &lcsm::model::TransmissionGate::wireSrcA() const noexcept
{
	return m_srcA;
}

const lcsm::model::Wire &lcsm::model::TransmissionGate::wireSrcB() const noexcept
{
	return m_srcB;
}

const lcsm::model::Wire &lcsm::model::TransmissionGate::wireSrcC() const noexcept
{
	return m_srcC;
}

lcsm::Identifier lcsm::model::TransmissionGate::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::TransmissionGate::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	lcsm::Identifier next = m_base.identify(m_id);
	next = m_srcA.identify(next);
	next = m_srcB.identify(next);
	next = m_srcC.identify(next);
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
		selected = std::addressof(m_base);
		break;
	case lcsm::model::TransmissionGate::Port::SrcA:
		selected = std::addressof(m_srcA);
		break;
	case lcsm::model::TransmissionGate::Port::SrcB:
		selected = std::addressof(m_srcB);
		break;
	case lcsm::model::TransmissionGate::Port::SrcC:
		selected = std::addressof(m_srcC);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
	selected->connectToWire(circuit);
}

void lcsm::model::TransmissionGate::connectBase(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::TransmissionGate::Port::Base, circuit);
}

void lcsm::model::TransmissionGate::connectSrcA(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::TransmissionGate::Port::SrcA, circuit);
}

void lcsm::model::TransmissionGate::connectSrcB(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::TransmissionGate::Port::SrcB, circuit);
}

void lcsm::model::TransmissionGate::connectSrcC(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::TransmissionGate::Port::SrcC, circuit);
}

lcsm::Circuit *lcsm::model::TransmissionGate::byPort(lcsm::portid_t portId)
{
	const lcsm::model::TransmissionGate::Port p = static_cast< lcsm::model::TransmissionGate::Port >(portId);
	switch (p)
	{
	case lcsm::model::TransmissionGate::Port::Base:
		return std::addressof(m_base);
	case lcsm::model::TransmissionGate::Port::SrcA:
		return std::addressof(m_srcA);
	case lcsm::model::TransmissionGate::Port::SrcB:
		return std::addressof(m_srcB);
	case lcsm::model::TransmissionGate::Port::SrcC:
		return std::addressof(m_srcC);
	}
	return nullptr;
}
