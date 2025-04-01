#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Component.h>
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

lcsm::model::Tunnel::Tunnel(lcsm::label_t name) : lcsm::Component(name) {}

lcsm::model::Tunnel::~Tunnel() noexcept
{
	disconnectAll();
}

const lcsm::model::Wire *lcsm::model::Tunnel::wire() const noexcept
{
	return m_wire.get();
}

const std::vector< lcsm::Component * > &lcsm::model::Tunnel::tunnels() const noexcept
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

lcsm::ComponentType lcsm::model::Tunnel::componentType() const noexcept
{
	return lcsm::ComponentType::Tunnel;
}

void lcsm::model::Tunnel::connect(lcsm::portid_t portId, lcsm::Component *circuit)
{
	const lcsm::model::Tunnel::Port p = static_cast< lcsm::model::Tunnel::Port >(portId);
	switch (p)
	{
	case lcsm::model::Tunnel::Port::Wiring:
		m_wire->connectToWire(circuit);
		break;
	case lcsm::model::Tunnel::Port::Tunneling:
		if (circuit->componentType() != componentType())
			throw std::logic_error("Can't connect non-Tunnel to Tunnel");
		m_tunnels.push_back(circuit);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Tunnel::disconnect(lcsm::Component *circuit) noexcept
{
	const std::vector< lcsm::Component * >::const_iterator found = std::find(m_tunnels.begin(), m_tunnels.end(), circuit);
	if (found != m_tunnels.end())
	{
		m_tunnels.erase(found);
	}
}

void lcsm::model::Tunnel::disconnectAll() noexcept
{
	m_wire->disconnectAll();
	for (lcsm::Component *tunnel : m_tunnels)
	{
		tunnel->disconnect(this);
	}
	m_tunnels.clear();
}

void lcsm::model::Tunnel::connectToWire(lcsm::Component *circuit)
{
	connect(lcsm::model::Tunnel::Port::Wiring, circuit);
}

void lcsm::model::Tunnel::connectTunnel(lcsm::Component *circuit)
{
	connect(lcsm::model::Tunnel::Port::Tunneling, circuit);
}

lcsm::Component *lcsm::model::Tunnel::byPort(lcsm::portid_t portId) noexcept
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

lcsm::portid_t lcsm::model::Tunnel::findPort(const lcsm::Component *circuit) const noexcept
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

void lcsm::model::Tunnel::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	// name <STRING>;
	writer.writeNameDeclaration(name());

	// keyvalue wireid <INTEGER>;
	writer.writeKeyValueDeclaration("wireid", wire()->id());

	builder.addWires(wire(), true);
	builder.addTunnels(this);
}

void lcsm::model::Tunnel::copy(lcsm::Component *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (componentType() != circuit->componentType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::Tunnel *tunnel = static_cast< lcsm::model::Tunnel * >(circuit);
	tunnel->setName(name());

	builder.oldToNew(id(), tunnel->id());
	builder.oldToNew(wire()->id(), tunnel->wire()->id());

	builder.addWires(wire(), true);
	builder.addTunnels(this);
}

void lcsm::model::Tunnel::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// name <STRING>;
	setName(reader.exceptName());

	// keyvalue wireid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("wireid")), wire()->id());
}
