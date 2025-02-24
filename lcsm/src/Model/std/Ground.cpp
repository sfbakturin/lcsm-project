#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Ground::Ground(lcsm::Width width) : m_width(width) {}

lcsm::model::Ground::~Ground() noexcept
{
	disconnectAll();
}

lcsm::Width lcsm::model::Ground::width() const noexcept
{
	return m_width;
}

void lcsm::model::Ground::setWidth(lcsm::Width width) noexcept
{
	m_width = width;
}

const lcsm::model::Wire *lcsm::model::Ground::wire() const noexcept
{
	return m_wire.get();
}

std::size_t lcsm::model::Ground::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Ground::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Ground::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Ground::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

lcsm::object_type_t lcsm::model::Ground::objectType() const noexcept
{
	return lcsm::ObjectType::Root | lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Ground::circuitType() const noexcept
{
	return lcsm::CircuitType::Ground;
}

void lcsm::model::Ground::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Ground::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Ground::disconnectAll() noexcept
{
	m_wire->disconnectAll();
}

void lcsm::model::Ground::connect(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Ground::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Ground::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Ground::Port p = static_cast< lcsm::model::Ground::Port >(portId);
	switch (p)
	{
	case lcsm::model::Ground::Port::Wiring:
		return m_wire.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Ground::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_wire.get())
		return lcsm::model::Ground::Port::Wiring;
	else
		return -1;
}
