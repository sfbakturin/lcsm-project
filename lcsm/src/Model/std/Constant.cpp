#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Constant::Constant(lcsm::Width width, lcsm::value_t value) : m_width(width), m_value(value) {}

lcsm::Width lcsm::model::Constant::width() const noexcept
{
	return m_width;
}

void lcsm::model::Constant::setWidth(lcsm::Width width) noexcept
{
	m_width = width;
}

lcsm::value_t lcsm::model::Constant::value() const noexcept
{
	return m_value;
}

void lcsm::model::Constant::setValue(lcsm::value_t value) noexcept
{
	m_value = value;
}

const lcsm::model::Wire *lcsm::model::Constant::wire() const noexcept
{
	return m_wire.get();
}

std::size_t lcsm::model::Constant::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Constant::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Constant::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Constant::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

lcsm::object_type_t lcsm::model::Constant::objectType() const noexcept
{
	return lcsm::ObjectType::Root | lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Constant::circuitType() const noexcept
{
	return lcsm::CircuitType::Constant;
}

void lcsm::model::Constant::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Constant::disconnect(lcsm::Circuit *)
{
	// Do nothing.
}

void lcsm::model::Constant::disconnectAll()
{
	m_wire->disconnectAll();
}

void lcsm::model::Constant::connect(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Constant::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Constant::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Constant::Port p = static_cast< lcsm::model::Constant::Port >(portId);
	switch (p)
	{
	case lcsm::model::Constant::Port::Wiring:
		return m_wire.get();
	}
	return nullptr;
}
