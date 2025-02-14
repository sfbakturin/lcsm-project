#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Button.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Button::Button(bool activeOnPress) : m_activeOnPress(activeOnPress)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
}

bool lcsm::model::Button::activeOnPress() const noexcept
{
	return m_activeOnPress;
}

void lcsm::model::Button::setActiveOnPress(bool activeOnPress) noexcept
{
	m_activeOnPress = activeOnPress;
}

const lcsm::model::Wire &lcsm::model::Button::wire() const noexcept
{
	return m_wire;
}

lcsm::Identifier lcsm::model::Button::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Button::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire.identify(m_id.next());
}

lcsm::ObjectType lcsm::model::Button::objectType() const noexcept
{
	return lcsm::ObjectType::IntExtIn;	  // FIXME: Button is not externally providable.
}

lcsm::CircuitType lcsm::model::Button::circuitType() const noexcept
{
	return lcsm::CircuitType::Button;
}

void lcsm::model::Button::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Button::Port p = static_cast< lcsm::model::Button::Port >(portId);
	switch (p)
	{
	case lcsm::model::Button::Port::Wiring:
		m_wire.connectToWire(circuit);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Button::connect(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Button::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Button::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Button::Port p = static_cast< lcsm::model::Button::Port >(portId);
	switch (p)
	{
	case lcsm::model::Button::Port::Wiring:
		return std::addressof(m_wire);
	}
	return nullptr;
}
