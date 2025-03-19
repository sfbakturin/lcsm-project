#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Button.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Button::Button(bool activeOnPress) : lcsm::model::Button("", activeOnPress) {}

lcsm::model::Button::Button(lcsm::label_t name, bool activeOnPress) :
	lcsm::Circuit(name), m_activeOnPress(activeOnPress)
{
}

lcsm::model::Button::~Button() noexcept
{
	disconnectAll();
}

bool lcsm::model::Button::activeOnPress() const noexcept
{
	return m_activeOnPress;
}

void lcsm::model::Button::setActiveOnPress(bool activeOnPress) noexcept
{
	m_activeOnPress = activeOnPress;
}

const lcsm::model::Wire *lcsm::model::Button::wire() const noexcept
{
	return m_wire.get();
}

std::size_t lcsm::model::Button::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Button::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Button::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Button::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

lcsm::object_type_t lcsm::model::Button::objectType() const noexcept
{
	return lcsm::ObjectType::Internal | lcsm::ObjectType::Input | lcsm::ObjectType::Root;
}

lcsm::CircuitType lcsm::model::Button::circuitType() const noexcept
{
	return lcsm::CircuitType::Button;
}

void lcsm::model::Button::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Button::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Button::disconnectAll() noexcept
{
	m_wire->disconnectAll();
}

void lcsm::model::Button::connect(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Button::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Button::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Button::Port p = static_cast< lcsm::model::Button::Port >(portId);
	switch (p)
	{
	case lcsm::model::Button::Port::Wiring:
		return m_wire.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Button::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_wire.get())
		return lcsm::model::Button::Port::Wiring;
	else
		return -1;
}

lcsm::portid_t lcsm::model::Button::defaultPort() const noexcept
{
	return lcsm::model::Button::Port::Wiring;
}

void lcsm::model::Button::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	// 'begincomponent'
	writer.writeBeginComponent();

	// circuittype <INTEGER>;
	writer.writeCircuitTypeDeclaration(circuitType());

	// id <IDENTIFIER>;
	writer.writeIdDeclaration(id());

	// name <STRING>;
	writer.writeNameDeclaration(name());

	// keyvalue activeOnPress <BOOLEAN>;
	writer.writeKeyValueDeclaration("activeOnPress", activeOnPress());

	// keyvalue wireid <INTEGER>;
	writer.writeKeyValueDeclaration("wireid", wire()->id());

	// Initialize wires.
	builder.addWires(m_wire.get(), true);

	// 'endcomponent'
	writer.writeEndComponent();
}

void lcsm::model::Button::copy(lcsm::Circuit *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (circuitType() != circuit->circuitType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::Button *button = static_cast< lcsm::model::Button * >(circuit);
	button->setName(name());
	button->setActiveOnPress(activeOnPress());

	builder.oldToNew(id(), button->id());
	builder.oldToNew(wire()->id(), button->wire()->id());

	builder.addWires(wire(), true);
}

void lcsm::model::Button::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// 'circuittype' is already parsed, so we continue to 'endcomponent'

	// id <IDENTIFIER>;
	builder.oldToNew(reader.exceptIdentifier(), id());

	// name <STRING>;
	setName(reader.exceptName());

	// keyvalue activeOnPress <BOOLEAN>;
	setActiveOnPress(reader.exceptBooleanKeyValue("activeOnPress"));

	// keyvalue wireid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("wireid")), wire()->id());
}
