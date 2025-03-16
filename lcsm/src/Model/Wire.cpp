#include "lcsm/Model/Builder.h"
#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Wire::Wire(lcsm::label_t name) : lcsm::Circuit(name) {}

lcsm::model::Wire::~Wire() noexcept
{
	disconnectAll();
}

const std::vector< lcsm::support::PointerView< lcsm::Circuit > > &lcsm::model::Wire::wires() const noexcept
{
	return m_wires;
}

const lcsm::support::PointerView< lcsm::Circuit > &lcsm::model::Wire::connect() const noexcept
{
	return m_connect;
}

std::size_t lcsm::model::Wire::numOfWires() const noexcept
{
	return 0;
}

void lcsm::model::Wire::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
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

lcsm::object_type_t lcsm::model::Wire::objectType() const noexcept
{
	return lcsm::ObjectType::Internal | lcsm::ObjectType::Wiring;
}

lcsm::CircuitType lcsm::model::Wire::circuitType() const noexcept
{
	return lcsm::CircuitType::Wire;
}

void lcsm::model::Wire::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	const lcsm::model::Wire::Port p = static_cast< lcsm::model::Wire::Port >(portId);

	/* If port is Wiring, then connect as wire, otherwise as connector. */
	switch (p)
	{
	case lcsm::model::Wire::Port::Wiring:
	{
		/* To Port::Wiring there is must be only Wire object. */
		if (circuit->objectType() != objectType())
			throw std::logic_error("Can't connect non-wiring object to wiring Wire's port.");
		m_wires.emplace_back(circuit);
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

void lcsm::model::Wire::disconnect(lcsm::Circuit *circuit) noexcept
{
	if (m_connect == circuit)
	{
		m_connect = nullptr;
		circuit->disconnect(this);
	}
	else
	{
		std::vector< lcsm::support::PointerView< lcsm::Circuit > >::iterator found =
			std::find(m_wires.begin(), m_wires.end(), circuit);
		if (found != m_wires.end())
		{
			m_wires.erase(found);
		}
	}
}

void lcsm::model::Wire::disconnectAll() noexcept
{
	for (lcsm::support::PointerView< lcsm::Circuit > &wire : m_wires)
		wire->disconnect(this);
	m_wires.clear();
}

void lcsm::model::Wire::connectToWire(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Wire::Port::Wiring, circuit);
}

void lcsm::model::Wire::connectConnect(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Wire::Port::Connect, circuit);
}

lcsm::Circuit *lcsm::model::Wire::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Wire::Port p = static_cast< lcsm::model::Wire::Port >(portId);
	switch (p)
	{
	case lcsm::model::Wire::Port::Wiring:
		return this;
	case lcsm::model::Wire::Port::Connect:
		return m_connect.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Wire::findPort(const lcsm::Circuit *) const noexcept
{
	return -1;
}

lcsm::portid_t lcsm::model::Wire::defaultPort() const noexcept
{
	return lcsm::model::Wire::Port::Wiring;
}

void lcsm::model::Wire::dumpToLCSMFile(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	writer.writeBeginComponent();
	writer.writeCircuitTypeDeclaration(circuitType());
	writer.writeIdDeclaration(m_id);
	writer.writeNameDeclaration(m_name);
	builder.addWires(this, false);
	writer.writeEndComponent();
}
