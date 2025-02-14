#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>
#include <vector>

const std::vector< lcsm::support::PointerView< lcsm::Circuit > > &lcsm::model::Wire::wires() const noexcept
{
	return m_wires;
}

const lcsm::support::PointerView< lcsm::Circuit > &lcsm::model::Wire::connect() const noexcept
{
	return m_connect;
}

lcsm::Identifier lcsm::model::Wire::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Wire::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_id.next();
}

lcsm::ObjectType lcsm::model::Wire::objectType() const noexcept
{
	return lcsm::ObjectType::Wiring;
}

lcsm::CircuitType lcsm::model::Wire::circuitType() const noexcept
{
	return lcsm::CircuitType::Wire;
}

void lcsm::model::Wire::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Wire::Port pw = static_cast< lcsm::model::Wire::Port >(portId);

	/* If port is Wiring, then connect as wire, otherwise as connector. */
	switch (pw)
	{
	case lcsm::model::Wire::Port::Wiring:
	{
		/* To Port::Wiring there is must be only Wire object. */
		if (circuit->objectType() != objectType())
			throw std::logic_error("Can't connect non-wiring object to wiring Wire's port.");
		m_wires.push_back(circuit);
		break;
	}
	case lcsm::model::Wire::Port::Connect:
	{
		m_connect = circuit;
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Wire::connectToWire(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Wire::Port::Wiring, circuit);
}

void lcsm::model::Wire::connectConnect(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Wire::Port::Connect, circuit);
}

lcsm::Circuit *lcsm::model::Wire::byPort(portid_t)
{
	return nullptr;
}
