#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Power::Power(lcsm::Width width) : m_width(width)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
}

lcsm::model::Power::Power(const lcsm::model::Power &other) : m_width(other.m_width)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
}

lcsm::model::Power::Power(lcsm::model::Power &&other) : m_width(other.m_width)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
}

lcsm::model::Power &lcsm::model::Power::operator=(const lcsm::model::Power &other)
{
	return lcsm::support::CopyAssign< lcsm::model::Power >(this, other);
}

lcsm::model::Power &lcsm::model::Power::operator=(lcsm::model::Power &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::model::Power >(this, std::move(other));
}

void lcsm::model::Power::swap(lcsm::model::Power &other) noexcept
{
	std::swap(m_width, other.m_width);
}

lcsm::Width lcsm::model::Power::width() const noexcept
{
	return m_width;
}

void lcsm::model::Power::setWidth(lcsm::Width newWidth) noexcept
{
	m_width = newWidth;
}

lcsm::Identifier lcsm::model::Power::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Power::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire.identify(m_id.next());
}

lcsm::ObjectType lcsm::model::Power::objectType() const noexcept
{
	return lcsm::ObjectType::RootInt;
}

lcsm::CircuitType lcsm::model::Power::circuitType() const noexcept
{
	return lcsm::CircuitType::Power;
}

void lcsm::model::Power::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Power::Port p = static_cast< lcsm::model::Power::Port >(portId);
	switch (p)
	{
	case lcsm::model::Power::Port::Wiring:
	{
		m_wire.connectToWire(circuit);
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Power::connect(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Power::Port::Wiring);
	connect(portId, circuit);
}

lcsm::Circuit *lcsm::model::Power::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Power::Port p = static_cast< lcsm::model::Power::Port >(portId);
	switch (p)
	{
	case lcsm::model::Power::Port::Wiring:
		return std::addressof(m_wire);
	}
	return nullptr;
}

const lcsm::model::Wire &lcsm::model::Power::wire() const noexcept
{
	return m_wire;
}
