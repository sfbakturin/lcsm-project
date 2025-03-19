#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Clock::Clock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset) :
	lcsm::model::Clock("", highDuration, lowDuration, phaseOffset)
{
}

lcsm::model::Clock::Clock(lcsm::label_t name, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset) :
	lcsm::Circuit(name), m_highDuration(highDuration), m_lowDuration(lowDuration), m_phaseOffset(phaseOffset)
{
}

lcsm::model::Clock::~Clock() noexcept
{
	disconnectAll();
}

unsigned lcsm::model::Clock::highDuration() const noexcept
{
	return m_highDuration;
}

void lcsm::model::Clock::setHighDuration(unsigned highDuration) noexcept
{
	m_highDuration = highDuration;
}

unsigned lcsm::model::Clock::lowDuration() const noexcept
{
	return m_lowDuration;
}

void lcsm::model::Clock::setLowDuration(unsigned lowDuration) noexcept
{
	m_lowDuration = lowDuration;
}

unsigned lcsm::model::Clock::phaseOffset() const noexcept
{
	return m_phaseOffset;
}

void lcsm::model::Clock::setPhaseOffset(unsigned phaseOffset) noexcept
{
	m_phaseOffset = phaseOffset;
}

const lcsm::model::Wire *lcsm::model::Clock::wire() const noexcept
{
	return m_wire.get();
}

std::size_t lcsm::model::Clock::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Clock::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Clock::id() const noexcept
{
	return m_id;
}

lcsm::object_type_t lcsm::model::Clock::objectType() const noexcept
{
	return lcsm::ObjectType::Root | lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Clock::circuitType() const noexcept
{
	return lcsm::CircuitType::Clock;
}

lcsm::Identifier lcsm::model::Clock::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

void lcsm::model::Clock::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *selected = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!selected)
		throw std::logic_error("Bad port!");
	selected->connectToWire(circuit);
}

void lcsm::model::Clock::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Clock::disconnectAll() noexcept
{
	m_wire->disconnectAll();
}

void lcsm::model::Clock::connect(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Clock::Port::Wiring, circuit);
}

lcsm::Circuit *lcsm::model::Clock::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Clock::Port p = static_cast< lcsm::model::Clock::Port >(portId);
	switch (p)
	{
	case lcsm::model::Clock::Port::Wiring:
		return m_wire.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Clock::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_wire.get())
		return lcsm::model::Clock::Port::Wiring;
	else
		return -1;
}

lcsm::portid_t lcsm::model::Clock::defaultPort() const noexcept
{
	return lcsm::model::Clock::Port::Wiring;
}

void lcsm::model::Clock::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	// begincomponent
	writer.writeBeginComponent();

	// circuittype <INTEGER>;
	writer.writeCircuitTypeDeclaration(circuitType());

	// id <INTEGER>;
	writer.writeIdDeclaration(id());

	// name <STRING>;
	writer.writeNameDeclaration(name());

	// keyvalue highDuration <INTEGER>;
	writer.writeKeyValueDeclaration("highDuration", static_cast< std::uint64_t >(highDuration()));

	// keyvalue lowDuration <INTEGER>;
	writer.writeKeyValueDeclaration("lowDuration", static_cast< std::uint64_t >(lowDuration()));

	// keyvalue phaseOffset <INTEGER>;
	writer.writeKeyValueDeclaration("phaseOffset", static_cast< std::uint64_t >(phaseOffset()));

	// keyvalue wireid <INTEGER>;
	writer.writeKeyValueDeclaration("wireid", wire()->id());

	builder.addWires(wire(), true);

	// endcomponent
	writer.writeEndComponent();
}

void lcsm::model::Clock::copy(lcsm::Circuit *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (circuitType() != circuit->circuitType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::Clock *clock = static_cast< lcsm::model::Clock * >(circuit);
	clock->setName(name());
	clock->setHighDuration(highDuration());
	clock->setLowDuration(lowDuration());
	clock->setPhaseOffset(phaseOffset());

	builder.oldToNew(id(), clock->id());
	builder.oldToNew(wire()->id(), clock->wire()->id());

	builder.addWires(wire(), true);
}

void lcsm::model::Clock::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// 'circuittype' is already parsed, so we continue to 'endcomponent'

	// id <IDENTIFIER>;
	builder.oldToNew(reader.exceptIdentifier(), id());

	// name <STRING>;
	setName(reader.exceptName());

	// keyvalue highDuration <INTEGER>;
	setHighDuration(static_cast< unsigned int >(reader.exceptIntegerKeyValue("highDuration")));

	// keyvalue lowDuration <INTEGER>;
	setLowDuration(static_cast< unsigned int >(reader.exceptIntegerKeyValue("lowDuration")));

	// keyvalue phaseOffset <INTEGER>;
	setPhaseOffset(static_cast< unsigned int >(reader.exceptIntegerKeyValue("phaseOffset")));

	// keyvalue wireid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("wireid")), wire()->id());
}
