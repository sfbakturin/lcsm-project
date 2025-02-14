#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Constant::Constant(lcsm::Width width, lcsm::value_t value) : m_width(width), m_value(value)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectToWire(circuit);
}

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

const lcsm::model::Wire &lcsm::model::Constant::wire() const noexcept
{
	return m_wire;
}

lcsm::Identifier lcsm::model::Constant::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Constant::identify(Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire.identify(m_id.next());
}

lcsm::ObjectType lcsm::model::Constant::objectType() const noexcept
{
	return lcsm::ObjectType::RootInt;
}

lcsm::CircuitType lcsm::model::Constant::circuitType() const noexcept
{
	return lcsm::CircuitType::Constant;
}

void lcsm::model::Constant::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Constant::Port p = static_cast< lcsm::model::Constant::Port >(portId);
	switch (p)
	{
	case lcsm::model::Constant::Port::Wiring:
		m_wire.connectToWire(circuit);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Constant::connect(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Constant::Port::Wiring);
	connect(portId, circuit);
}

lcsm::Circuit *lcsm::model::Constant::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Constant::Port p = static_cast< lcsm::model::Constant::Port >(portId);
	switch (p)
	{
	case lcsm::model::Constant::Port::Wiring:
		return std::addressof(m_wire);
	}
	return nullptr;
}
