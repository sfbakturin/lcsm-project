#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Tunnel::Tunnel()
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
}

const lcsm::model::Wire &lcsm::model::Tunnel::wire() const noexcept
{
	return m_wire;
}

const lcsm::support::PointerView< lcsm::Circuit > &lcsm::model::Tunnel::tunnel() const noexcept
{
	return m_tunnel;
}

lcsm::Identifier lcsm::model::Tunnel::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Tunnel::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire.identify(m_id.next());
}

lcsm::ObjectType lcsm::model::Tunnel::objectType() const noexcept
{
	return lcsm::ObjectType::PureInt;
}

lcsm::CircuitType lcsm::model::Tunnel::circuitType() const noexcept
{
	return lcsm::CircuitType::Tunnel;
}

void lcsm::model::Tunnel::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Tunnel::Port p = static_cast< lcsm::model::Tunnel::Port >(portId);
	switch (p)
	{
	case lcsm::model::Tunnel::Port::Wiring:
		m_wire.connectToWire(circuit);
		break;
	case lcsm::model::Tunnel::Port::Tunneling:
		m_tunnel = circuit;
		break;
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Tunnel::connectToWire(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Tunnel::Port::Wiring, circuit);
}

void lcsm::model::Tunnel::connectTunnel(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Tunnel::Port::Tunneling, circuit);
}

lcsm::Circuit *lcsm::model::Tunnel::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Tunnel::Port p = static_cast< lcsm::model::Tunnel::Port >(portId);
	switch (p)
	{
	case lcsm::model::Tunnel::Port::Wiring:
		return std::addressof(m_wire);
	case lcsm::model::Tunnel::Port::Tunneling:
		return m_tunnel.ptr();
	}
	return nullptr;
}
