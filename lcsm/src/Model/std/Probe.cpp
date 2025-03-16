#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Probe.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

lcsm::model::Probe::Probe(lcsm::label_t name) : lcsm::Circuit(name) {}

lcsm::model::Probe::~Probe() noexcept
{
	disconnectAll();
}

const lcsm::model::Wire *lcsm::model::Probe::wire() const noexcept
{
	return m_wire.get();
}

std::size_t lcsm::model::Probe::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Probe::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Probe::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Probe::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

lcsm::object_type_t lcsm::model::Probe::objectType() const noexcept
{
	return lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Probe::circuitType() const noexcept
{
	return lcsm::CircuitType::Probe;
}

void lcsm::model::Probe::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Probe::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Probe::disconnectAll() noexcept
{
	m_wire->disconnectAll();
}

void lcsm::model::Probe::connect(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Probe::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Probe::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Probe::Port p = static_cast< lcsm::model::Probe::Port >(portId);
	switch (p)
	{
	case lcsm::model::Probe::Port::Wiring:
		return m_wire.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Probe::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_wire.get())
		return lcsm::model::Probe::Port::Wiring;
	else
		return -1;
}

lcsm::portid_t lcsm::model::Probe::defaultPort() const noexcept
{
	return lcsm::model::Probe::Port::Wiring;
}

void lcsm::model::Probe::dumpToLCSMFile(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	writer.writeBeginComponent();
	writer.writeCircuitTypeDeclaration(circuitType());
	writer.writeIdDeclaration(m_id);
	writer.writeNameDeclaration(m_name);
	writer.writeKeyValueDeclaration("wireid", m_wire->id());
	builder.addWires(m_wire.get(), true);
	writer.writeEndComponent();
}
