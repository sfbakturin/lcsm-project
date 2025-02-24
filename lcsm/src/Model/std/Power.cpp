#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Power::Power(lcsm::Width width) : m_width(width) {}

lcsm::model::Power::~Power() noexcept
{
	disconnectAll();
}

lcsm::Width lcsm::model::Power::width() const noexcept
{
	return m_width;
}

void lcsm::model::Power::setWidth(lcsm::Width width) noexcept
{
	m_width = width;
}

const lcsm::model::Wire *lcsm::model::Power::wire() const noexcept
{
	return m_wire.get();
}

std::size_t lcsm::model::Power::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Power::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Power::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Power::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

lcsm::object_type_t lcsm::model::Power::objectType() const noexcept
{
	return lcsm::ObjectType::Root | lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Power::circuitType() const noexcept
{
	return lcsm::CircuitType::Power;
}

void lcsm::model::Power::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Power::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Power::disconnectAll() noexcept
{
	m_wire->disconnectAll();
}

void lcsm::model::Power::connect(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Power::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Power::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Power::Port p = static_cast< lcsm::model::Power::Port >(portId);
	switch (p)
	{
	case lcsm::model::Power::Port::Wiring:
		return m_wire.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Power::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_wire.get())
		return lcsm::model::Power::Port::Wiring;
	else
		return -1;
}
