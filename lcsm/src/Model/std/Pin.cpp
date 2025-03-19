#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Pin::Pin(bool output, lcsm::Width width) : lcsm::model::Pin("", output, width) {}

lcsm::model::Pin::Pin(lcsm::label_t name, bool output, lcsm::Width width) :
	lcsm::Circuit(name), m_output(output), m_width(width)
{
}

lcsm::model::Pin::~Pin() noexcept
{
	disconnectAll();
}

bool lcsm::model::Pin::output() const noexcept
{
	return m_output;
}

void lcsm::model::Pin::setOutput(bool output) noexcept
{
	m_output = output;
}

lcsm::Width lcsm::model::Pin::width() const noexcept
{
	return m_width;
}

void lcsm::model::Pin::setWidth(lcsm::Width width) noexcept
{
	m_width = width;
}

const lcsm::model::Wire *lcsm::model::Pin::internal() const noexcept
{
	return m_internal.get();
}

const lcsm::model::Wire *lcsm::model::Pin::external() const noexcept
{
	return m_external.get();
}

std::size_t lcsm::model::Pin::numOfWires() const noexcept
{
	return 2;
}

void lcsm::model::Pin::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad number of wires!");
	if (m_internal)
		m_internal->disconnect(this);
	if (m_external)
		m_external->disconnect(this);
	m_internal = wires[0];
	m_external = wires[1];
	m_internal->connectConnect(this);
	m_external->connectConnect(this);
}

lcsm::Identifier lcsm::model::Pin::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Pin::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	const lcsm::Identifier next = m_internal->identify(m_id.next());
	return m_external->identify(next);
}

lcsm::object_type_t lcsm::model::Pin::objectType() const noexcept
{
	return lcsm::ObjectType::Internal | lcsm::ObjectType::External |
		   (m_output ? lcsm::ObjectType::Output : lcsm::ObjectType::Input | lcsm::ObjectType::Root);
}

lcsm::CircuitType lcsm::model::Pin::circuitType() const noexcept
{
	return lcsm::CircuitType::Pin;
}

void lcsm::model::Pin::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *selected = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!selected)
		throw std::logic_error("Bad port!");
	selected->connectToWire(circuit);
}

void lcsm::model::Pin::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Pin::disconnectAll() noexcept
{
	m_internal->disconnectAll();
	m_external->disconnectAll();
}

lcsm::Circuit *lcsm::model::Pin::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Pin::Port p = static_cast< lcsm::model::Pin::Port >(portId);
	switch (p)
	{
	case lcsm::model::Pin::Port::Internal:
		return m_internal.get();
	case lcsm::model::Pin::Port::External:
		return m_external.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Pin::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_internal.get())
		return lcsm::model::Pin::Port::Internal;
	else if (circuit == m_external.get())
		return lcsm::model::Pin::Port::External;
	else
		return -1;
}

lcsm::portid_t lcsm::model::Pin::defaultPort() const noexcept
{
	return lcsm::model::Pin::Port::Internal;
}

void lcsm::model::Pin::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	// begincomponent
	writer.writeBeginComponent();

	// circuittype <INTEGER>;
	writer.writeCircuitTypeDeclaration(circuitType());

	// id <IDENTIFIER>;
	writer.writeIdDeclaration(id());

	// name <STRING>;
	writer.writeNameDeclaration(name());

	// keyvalue output <BOOLEAN>;
	writer.writeKeyValueDeclaration("output", output());

	// keyvalue width <INTEGER>;
	writer.writeKeyValueDeclaration("width", static_cast< std::uint64_t >(width()));

	// keyvalue internalid <INTEGER>;
	writer.writeKeyValueDeclaration("internalid", internal()->id());

	// keyvalue externalid <INTEGER>;
	writer.writeKeyValueDeclaration("externalid", external()->id());

	builder.addWires(internal(), true);
	builder.addWires(external(), true);

	// endcomponent
	writer.writeEndComponent();
}

void lcsm::model::Pin::copy(lcsm::Circuit *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (circuitType() != circuit->circuitType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::Pin *pin = static_cast< lcsm::model::Pin * >(circuit);
	pin->setName(name());
	pin->setOutput(output());
	pin->setWidth(width());

	builder.oldToNew(id(), pin->id());
	builder.oldToNew(internal()->id(), pin->internal()->id());
	builder.oldToNew(external()->id(), pin->external()->id());

	builder.addWires(internal(), true);
	builder.addWires(external(), true);
}

void lcsm::model::Pin::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// 'circuittype' is already parsed, so we continue to 'endcomponent'

	// id <IDENTIFIER>;
	const lcsm::Identifier oldId = reader.exceptIdentifier();
	builder.oldToNew(oldId, id());

	// name <STRING>;
	setName(reader.exceptName());

	// keyvalue output <BOOLEAN>;
	setOutput(reader.exceptBooleanKeyValue("output"));

	// keyvalue width <INTEGER>;
	setWidth(lcsm::StrongToWidth(reader.exceptIntegerKeyValue("width")));

	// keyvalue externalid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("internalid")), internal()->id());

	// keyvalue internalid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("externalid")), external()->id());
}
