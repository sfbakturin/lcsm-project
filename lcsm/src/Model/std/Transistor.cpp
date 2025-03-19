#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Transistor::Transistor(lcsm::model::Transistor::Type type) : lcsm::model::Transistor("", type) {}

lcsm::model::Transistor::Transistor(lcsm::label_t name, lcsm::model::Transistor::Type type) :
	lcsm::Circuit(name), m_type(type)
{
}

lcsm::model::Transistor::~Transistor() noexcept
{
	disconnectAll();
}

lcsm::model::Transistor::Type lcsm::model::Transistor::type() const noexcept
{
	return m_type;
}

void lcsm::model::Transistor::setType(lcsm::model::Transistor::Type type) noexcept
{
	m_type = type;
}

const lcsm::model::Wire *lcsm::model::Transistor::wireBase() const noexcept
{
	return m_base.get();
}

const lcsm::model::Wire *lcsm::model::Transistor::wireSrcA() const noexcept
{
	return m_srcA.get();
}

const lcsm::model::Wire *lcsm::model::Transistor::wireSrcB() const noexcept
{
	return m_srcB.get();
}

std::size_t lcsm::model::Transistor::numOfWires() const noexcept
{
	return 3;
}

void lcsm::model::Transistor::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_base = wires[0];
	m_srcA = wires[1];
	m_srcB = wires[2];
	m_base->connectConnect(this);
	m_srcA->connectConnect(this);
	m_srcB->connectConnect(this);
}

lcsm::Identifier lcsm::model::Transistor::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Transistor::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	lcsm::Identifier next = m_base->identify(m_id.next());
	next = m_srcA->identify(next);
	next = m_srcB->identify(next);
	return next;
}

lcsm::object_type_t lcsm::model::Transistor::objectType() const noexcept
{
	return lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Transistor::circuitType() const noexcept
{
	return lcsm::CircuitType::Transistor;
}

void lcsm::model::Transistor::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Transistor::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Transistor::disconnectAll() noexcept
{
	m_base->disconnectAll();
	m_srcA->disconnectAll();
	m_srcB->disconnectAll();
}

void lcsm::model::Transistor::connectBase(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Transistor::Port::Base, circuit);
}

void lcsm::model::Transistor::connectSrcA(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Transistor::Port::SrcA, circuit);
}

void lcsm::model::Transistor::connectSrcB(lcsm::Circuit *circuit)
{
	connect(lcsm::model::Transistor::Port::SrcB, circuit);
}

lcsm::Circuit *lcsm::model::Transistor::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Transistor::Port p = static_cast< lcsm::model::Transistor::Port >(portId);
	switch (p)
	{
	case lcsm::model::Transistor::Port::Base:
		return m_base.get();
	case lcsm::model::Transistor::Port::SrcA:
		return m_srcA.get();
	case lcsm::model::Transistor::Port::SrcB:
		return m_srcB.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Transistor::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_base.get())
		return lcsm::model::Transistor::Port::Base;
	else if (circuit == m_srcA.get())
		return lcsm::model::Transistor::Port::SrcA;
	else if (circuit == m_srcB.get())
		return lcsm::model::Transistor::Port::SrcB;
	else
		return -1;
}

lcsm::portid_t lcsm::model::Transistor::defaultPort() const noexcept
{
	return lcsm::model::Transistor::Port::Base;
}

void lcsm::model::Transistor::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	writer.writeBeginComponent();
	writer.writeCircuitTypeDeclaration(circuitType());
	writer.writeIdDeclaration(id());
	writer.writeNameDeclaration(name());
	writer.writeKeyValueDeclaration("type", static_cast< std::uint64_t >(type()));
	writer.writeKeyValueDeclaration("baseid", wireBase()->id());
	writer.writeKeyValueDeclaration("srcaid", wireSrcA()->id());
	writer.writeKeyValueDeclaration("srcbid", wireSrcB()->id());
	builder.addWires(wireBase(), true);
	builder.addWires(wireSrcA(), true);
	builder.addWires(wireSrcB(), true);
	writer.writeEndComponent();
}

void lcsm::model::Transistor::copy(lcsm::Circuit *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (circuitType() != circuit->circuitType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::Transistor *transistor = static_cast< lcsm::model::Transistor * >(circuit);
	transistor->setName(name());
	transistor->setType(type());

	builder.oldToNew(id(), transistor->id());
	builder.oldToNew(wireBase()->id(), transistor->wireBase()->id());
	builder.oldToNew(wireSrcA()->id(), transistor->wireSrcA()->id());
	builder.oldToNew(wireSrcB()->id(), transistor->wireSrcB()->id());

	builder.addWires(wireBase(), true);
	builder.addWires(wireSrcA(), true);
	builder.addWires(wireSrcB(), true);
}

void lcsm::model::Transistor::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// 'circuittype' is already parsed, so we continue to 'endcomponent'

	// id <IDENTIFIER>;
	builder.oldToNew(reader.exceptIdentifier(), id());

	// name <STRING>;
	setName(reader.exceptName());

	// keyvalue type <INTEGER>;
	const unsigned long long type = reader.exceptIntegerKeyValue("type");
	if (type != lcsm::model::Transistor::Type::P && type != lcsm::model::Transistor::Type::N)
	{
		throw std::logic_error("Mistaken Transistor's type.");
	}
	setType(static_cast< lcsm::model::Transistor::Type >(type));

	// keyvalue baseid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("baseid")), wireBase()->id());

	// keyvalue srcaid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("srcaid")), wireSrcA()->id());

	// keyvalue srcbid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("srcbid")), wireSrcB()->id());
}
