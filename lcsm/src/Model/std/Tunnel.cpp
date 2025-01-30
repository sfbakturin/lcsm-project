#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>

lcsm::model::Tunnel::Tunnel()
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
}

lcsm::model::Tunnel::Tunnel(const lcsm::model::Tunnel &) {}

lcsm::model::Tunnel::Tunnel(lcsm::model::Tunnel &&) noexcept {}

lcsm::model::Tunnel &lcsm::model::Tunnel::operator=(const lcsm::model::Tunnel &other)
{
	return lcsm::support::CopyAssign< lcsm::model::Tunnel >(this, other);
}

lcsm::model::Tunnel &lcsm::model::Tunnel::operator=(lcsm::model::Tunnel &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::model::Tunnel >(this, std::move(other));
}

void lcsm::model::Tunnel::swap(lcsm::model::Tunnel &other) noexcept
{
	std::swap(m_wire, other.m_wire);
	std::swap(m_tunnel, other.m_tunnel);
	std::swap(m_id, other.m_id);
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
	const lcsm::model::Tunnel::Port pt = static_cast< lcsm::model::Tunnel::Port >(portId);
	switch (pt)
	{
	case lcsm::model::Tunnel::Port::Wiring:
	{
		m_wire.connectToWire(circuit);
		break;
	}
	case lcsm::model::Tunnel::Port::Tunneling:
	{
		m_tunnel = circuit;
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Tunnel::connectToWire(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Tunnel::Port::Wiring);
	connect(portId, circuit);
}

void lcsm::model::Tunnel::connectTunnel(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Tunnel::Port::Tunneling);
	connect(portId, circuit);
}
