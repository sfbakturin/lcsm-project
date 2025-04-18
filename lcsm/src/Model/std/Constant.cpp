#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Constant::Constant(lcsm::Width width, lcsm::value_t value) : lcsm::model::Constant("", width, value) {}

lcsm::model::Constant::Constant(lcsm::label_t name, lcsm::Width width, lcsm::value_t value) :
	lcsm::Component(name), m_width(width), m_value(value)
{
}

lcsm::model::Constant::~Constant() noexcept
{
	disconnectAll();
}

lcsm::Width lcsm::model::Constant::width() const noexcept
{
	return m_width;
}

static constexpr lcsm::value_t GetValue(lcsm::Width width) noexcept
{
	return (std::numeric_limits< lcsm::value_t >::max() >> (63 - (width - 1)));
}

void lcsm::model::Constant::setWidth(lcsm::Width width) noexcept
{
	m_width = width;
	m_value = GetValue(width);
}

lcsm::value_t lcsm::model::Constant::value() const noexcept
{
	return m_value;
}

void lcsm::model::Constant::setValue(lcsm::value_t value) noexcept
{
	m_value = value;
}

const lcsm::model::Wire *lcsm::model::Constant::wire() const noexcept
{
	return m_wire.get();
}

std::size_t lcsm::model::Constant::numOfWires() const noexcept
{
	return 1;
}

void lcsm::model::Constant::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wire = wires[0];
	m_wire->connectConnect(this);
}

lcsm::Identifier lcsm::model::Constant::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Constant::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire->identify(m_id.next());
}

lcsm::object_type_t lcsm::model::Constant::objectType() const noexcept
{
	return lcsm::ObjectType::Root | lcsm::ObjectType::Internal;
}

lcsm::ComponentType lcsm::model::Constant::componentType() const noexcept
{
	return lcsm::ComponentType::Constant;
}

void lcsm::model::Constant::connect(lcsm::portid_t portId, lcsm::Component *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

void lcsm::model::Constant::disconnect(lcsm::Component *) noexcept
{
	// Do nothing.
}

void lcsm::model::Constant::disconnectAll() noexcept
{
	m_wire->disconnectAll();
}

void lcsm::model::Constant::connect(lcsm::Component *circuit)
{
	connect(lcsm::model::Constant::Port::Wiring, circuit);
}

lcsm::Component *lcsm::model::Constant::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Constant::Port p = static_cast< lcsm::model::Constant::Port >(portId);
	switch (p)
	{
	case lcsm::model::Constant::Port::Wiring:
		return m_wire.get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Constant::findPort(const lcsm::Component *circuit) const noexcept
{
	if (circuit == m_wire.get())
		return lcsm::model::Constant::Port::Wiring;
	else
		return -1;
}

lcsm::portid_t lcsm::model::Constant::defaultPort() const noexcept
{
	return lcsm::model::Constant::Port::Wiring;
}

void lcsm::model::Constant::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	// name <STRING>;
	writer.writeNameDeclaration(name());

	// keyvalue width <INTEGER>;
	writer.writeKeyValueDeclaration("width", static_cast< std::uint64_t >(width()));

	// keyvalue value <INTEGER>;
	writer.writeKeyValueDeclaration("value", value());

	// keyvalue wireid <INTEGER>;
	writer.writeKeyValueDeclaration("wireid", wire()->id());

	builder.addWires(wire(), true);
}

void lcsm::model::Constant::copy(lcsm::Component *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (componentType() != circuit->componentType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::Constant *constant = static_cast< lcsm::model::Constant * >(circuit);
	constant->setName(name());
	constant->setWidth(width());
	constant->setValue(value());

	builder.oldToNew(id(), constant->id());
	builder.oldToNew(wire()->id(), constant->wire()->id());

	builder.addWires(wire(), true);
}

void lcsm::model::Constant::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// name <STRING>;
	setName(reader.exceptName());

	// keyvalue width <INTEGER>;
	setWidth(lcsm::StrongToWidth(reader.exceptIntegerKeyValue("width")));

	// keyvalue value <INTEGER>;
	setValue(static_cast< lcsm::value_t >(reader.exceptIntegerKeyValue("value")));

	// keyvalue wireid <INTEGER>;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("wireid")), wire()->id());
}
