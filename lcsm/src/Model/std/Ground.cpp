#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <utility>

lcsm::model::Ground::Ground(lcsm::Width width) : m_width(width)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
}

lcsm::Width lcsm::model::Ground::width() const noexcept
{
	return m_width;
}

void lcsm::model::Ground::setWidth(lcsm::Width width) noexcept
{
	m_width = width;
}

const lcsm::model::Wire &lcsm::model::Ground::wire() const noexcept
{
	return m_wire;
}

lcsm::Identifier lcsm::model::Ground::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Ground::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire.identify(m_id.next());
}

lcsm::ObjectType lcsm::model::Ground::objectType() const noexcept
{
	return lcsm::ObjectType::RootInt;
}

lcsm::CircuitType lcsm::model::Ground::circuitType() const noexcept
{
	return lcsm::CircuitType::Power;
}

void lcsm::model::Ground::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Ground::Port p = static_cast< lcsm::model::Ground::Port >(portId);
	switch (p)
	{
	case lcsm::model::Ground::Port::Wiring:
	{
		m_wire.connectToWire(circuit);
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Ground::connect(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Ground::Port::Wiring);
	connect(portId, circuit);
}

lcsm::Circuit *lcsm::model::Ground::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Ground::Port p = static_cast< lcsm::model::Ground::Port >(portId);
	switch (p)
	{
	case lcsm::model::Ground::Port::Wiring:
		return std::addressof(m_wire);
	}
	return nullptr;
}
