#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Power::Power(lcsm::Width width) : lcsm::model::Power("", width) {}

lcsm::model::Power::Power(lcsm::label_t name, lcsm::Width width) : lcsm::Circuit(name), m_width(width) {}

lcsm::model::Power::~Power() noexcept
{
	disconnectAll();
}

lcsm::Width lcsm::model::Power::width() const noexcept
{
	return m_width;
}

void lcsm::model::Power::setWidth(lcsm::Width width) noexcept
{
	m_width = width;
}

const lcsm::model::Wire *lcsm::model::Power::wire() const noexcept
{
	return m_wire.get();
}

std::size_t lcsm::model::Power::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Power::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Power::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Power::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

lcsm::object_type_t lcsm::model::Power::objectType() const noexcept
{
	return lcsm::ObjectType::Root | lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Power::circuitType() const noexcept
{
	return lcsm::CircuitType::Power;
}

void lcsm::model::Power::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Power::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Power::disconnectAll() noexcept
{
	m_wire->disconnectAll();
}

void lcsm::model::Power::connect(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Power::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Power::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Power::Port p = static_cast< lcsm::model::Power::Port >(portId);
	switch (p)
	{
	case lcsm::model::Power::Port::Wiring:
		return m_wire.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Power::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_wire.get())
		return lcsm::model::Power::Port::Wiring;
	else
		return -1;
}

lcsm::portid_t lcsm::model::Power::defaultPort() const noexcept
{
	return lcsm::model::Power::Port::Wiring;
}

void lcsm::model::Power::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	// begincomponent
	writer.writeBeginComponent();

	// circuittype <INTEGER>;
	writer.writeCircuitTypeDeclaration(circuitType());

	// id <IDENTIFIER>;
	writer.writeIdDeclaration(id());

	// name <STRING>;
	writer.writeNameDeclaration(name());

	// keyvalue width <INTEGER>;
	writer.writeKeyValueDeclaration("width", static_cast< std::uint64_t >(width()));

	// keyvalue wireid <INTEGER>;
	writer.writeKeyValueDeclaration("wireid", wire()->id());

	builder.addWires(m_wire.get(), true);

	// endcomponent
	writer.writeEndComponent();
}

void lcsm::model::Power::copy(lcsm::Circuit *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (circuitType() != circuit->circuitType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::Power *power = static_cast< lcsm::model::Power * >(circuit);
	power->setName(name());
	power->setWidth(width());

	builder.oldToNew(id(), power->id());
	builder.oldToNew(wire()->id(), power->wire()->id());

	builder.addWires(wire(), true);
}

void lcsm::model::Power::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// 'circuittype' is already parsed, so we continue to 'endcomponent'

	// id <IDENTIFIER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIdentifier()), id());

	// name <STRING>;
	setName(reader.exceptName());

	// keyvalue width <INTEGER>;
	setWidth(lcsm::StrongToWidth(reader.exceptIntegerKeyValue("width")));

	// keyvalue wireid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("wireid")), wire()->id());
}
