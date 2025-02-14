#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Probe.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>

lcsm::model::Probe::Probe()
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
}

const lcsm::model::Wire &lcsm::model::Probe::wire() const noexcept
{
	return m_wire;
}

lcsm::Identifier lcsm::model::Probe::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Probe::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire.identify(m_id.next());
}

lcsm::ObjectType lcsm::model::Probe::objectType() const noexcept
{
	return lcsm::ObjectType::PureInt;
}

lcsm::CircuitType lcsm::model::Probe::circuitType() const noexcept
{
	return lcsm::CircuitType::Probe;
}

void lcsm::model::Probe::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Probe::Port p = static_cast< lcsm::model::Probe::Port >(portId);
	switch (p)
	{
	case lcsm::model::Probe::Port::Wiring:
		m_wire.connectToWire(circuit);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Probe::connect(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Probe::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Probe::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Probe::Port p = static_cast< lcsm::model::Probe::Port >(portId);
	switch (p)
	{
	case lcsm::model::Probe::Port::Wiring:
		return std::addressof(m_wire);
	}
	return nullptr;
}
