#include "lcsm/Model/Builder.h"
#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Tunnel::Tunnel(lcsm::label_t name) : lcsm::Circuit(name) {}

lcsm::model::Tunnel::~Tunnel() noexcept
{
	disconnectAll();
}

const lcsm::model::Wire *lcsm::model::Tunnel::wire() const noexcept
{
	return m_wire.get();
}

const std::vector< lcsm::Circuit * > &lcsm::model::Tunnel::tunnels() const noexcept
{
	return m_tunnels;
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
	const lcsm::model::Tunnel::Port p = static_cast< lcsm::model::Tunnel::Port >(portId);
	switch (p)
	{
	case lcsm::model::Tunnel::Port::Wiring:
		m_wire->connectToWire(circuit);
		break;
	case lcsm::model::Tunnel::Port::Tunneling:
		if (circuit->circuitType() != circuitType())
			throw std::logic_error("Can't connect non-Tunnel to Tunnel");
		m_tunnels.push_back(circuit);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Tunnel::disconnect(lcsm::Circuit *circuit) noexcept
{
	const std::vector< lcsm::Circuit * >::const_iterator found = std::find(m_tunnels.begin(), m_tunnels.end(), circuit);
	if (found != m_tunnels.end())
	{
		m_tunnels.erase(found);
	}
}

void lcsm::model::Tunnel::disconnectAll() noexcept
{
	m_wire->disconnectAll();
	for (lcsm::Circuit *tunnel : m_tunnels)
	{
		tunnel->disconnect(this);
	}
	m_tunnels.clear();
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
		return this;
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

lcsm::portid_t lcsm::model::Tunnel::defaultPort() const noexcept
{
	return lcsm::model::Tunnel::Port::Wiring;
}

void lcsm::model::Tunnel::dumpToLCSMFile(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	writer.writeBeginComponent();
	writer.writeCircuitTypeDeclaration(circuitType());
	writer.writeIdDeclaration(m_id);
	writer.writeNameDeclaration(m_name);
	writer.writeKeyValueDeclaration("wireid", m_wire->id());
	builder.addWires(m_wire.get(), true);
	builder.addTunnels(this);
	writer.writeEndComponent();
}
