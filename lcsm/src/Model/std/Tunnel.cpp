#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Tunnel::~Tunnel() noexcept
{
	disconnectAll();
}

const lcsm::model::Wire *lcsm::model::Tunnel::wire() const noexcept
{
	return m_wire.get();
}

const lcsm::support::PointerView< lcsm::Circuit > &lcsm::model::Tunnel::tunnel() const noexcept
{
	return m_tunnel;
}

std::size_t lcsm::model::Tunnel::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Tunnel::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Tunnel::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Tunnel::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

lcsm::object_type_t lcsm::model::Tunnel::objectType() const noexcept
{
	return lcsm::ObjectType::Internal | lcsm::ObjectType::Wiring;
}

lcsm::CircuitType lcsm::model::Tunnel::circuitType() const noexcept
{
	return lcsm::CircuitType::Tunnel;
}

void lcsm::model::Tunnel::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Tunnel::disconnect(lcsm::Circuit *) noexcept
{
	// TODO: Implement me.
}

void lcsm::model::Tunnel::disconnectAll() noexcept
{
	m_wire->disconnectAll();
	m_tunnel->disconnectAll();
}

void lcsm::model::Tunnel::connectToWire(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Tunnel::Port::Wiring, circuit);
}

void lcsm::model::Tunnel::connectTunnel(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Tunnel::Port::Tunneling, circuit);
}

lcsm::Circuit *lcsm::model::Tunnel::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Tunnel::Port p = static_cast< lcsm::model::Tunnel::Port >(portId);
	switch (p)
	{
	case lcsm::model::Tunnel::Port::Wiring:
		return m_wire.get();
	case lcsm::model::Tunnel::Port::Tunneling:
		return m_tunnel.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Tunnel::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_wire.get())
		return lcsm::model::Tunnel::Port::Wiring;
	else
		return -1;
}
