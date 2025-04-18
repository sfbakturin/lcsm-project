#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Digit.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::model::Digit::Digit(bool hasDecimalPoint) : lcsm::model::Digit("", hasDecimalPoint) {}

lcsm::model::Digit::Digit(lcsm::label_t name, bool hasDecimalPoint) :
	lcsm::Component(name), m_hasDecimalPoint(hasDecimalPoint)
{
}

lcsm::model::Digit::~Digit() noexcept
{
	disconnectAll();
}

bool lcsm::model::Digit::hasDecimalPoint() const noexcept
{
	return m_hasDecimalPoint;
}

void lcsm::model::Digit::setHasDecimalPoint(bool hasDecimalPoint) noexcept
{
	m_hasDecimalPoint = hasDecimalPoint;
	if (!hasDecimalPoint)
		m_wireDecimalPoint->disconnectAll();
}

const lcsm::model::Wire *lcsm::model::Digit::wireData() const noexcept
{
	return m_wireData.get();
}

const lcsm::model::Wire *lcsm::model::Digit::wireDecimalPoint() const noexcept
{
	return m_wireDecimalPoint.get();
}

std::size_t lcsm::model::Digit::numOfWires() const noexcept
{
	return 2;
}

void lcsm::model::Digit::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	m_wireData = wires[0];
	m_wireDecimalPoint = wires[1];
	m_wireData->connectConnect(this);
	m_wireDecimalPoint->connectConnect(this);
}

lcsm::Identifier lcsm::model::Digit::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Digit::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wireDecimalPoint->identify(m_wireData->identify(m_id.next()));
}

lcsm::object_type_t lcsm::model::Digit::objectType() const noexcept
{
	return lcsm::ObjectType::Internal;
}

lcsm::ComponentType lcsm::model::Digit::componentType() const noexcept
{
	return lcsm::ComponentType::Digit;
}

void lcsm::model::Digit::connect(lcsm::portid_t portId, lcsm::Component *circuit)
{
	const lcsm::model::Digit::Port p = static_cast< lcsm::model::Digit::Port >(portId);
	switch (p)
	{
	case lcsm::model::Digit::Port::WiringData:
	{
		m_wireData->connectToWire(circuit);
		break;
	}
	case lcsm::model::Digit::Port::WiringDecimalPoint:
	{
		if (!m_hasDecimalPoint)
			throw std::logic_error("Digit doesn't have port for decimal point!");
		m_wireDecimalPoint->connectToWire(circuit);
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Digit::disconnect(lcsm::Component *) noexcept
{
	// Do nothing.
}

void lcsm::model::Digit::disconnectAll() noexcept
{
	m_wireData->disconnectAll();
	m_wireDecimalPoint->disconnectAll();
}

void lcsm::model::Digit::connectData(lcsm::Component *circuit)
{
	connect(lcsm::model::Digit::Port::WiringData, circuit);
}

void lcsm::model::Digit::connectDecimalPoint(lcsm::Component *circuit)
{
	connect(lcsm::model::Digit::Port::WiringDecimalPoint, circuit);
}

lcsm::Component *lcsm::model::Digit::byPort(lcsm::portid_t portId) noexcept
{
	const lcsm::model::Digit::Port p = static_cast< lcsm::model::Digit::Port >(portId);
	switch (p)
	{
	case lcsm::model::Digit::Port::WiringData:
		return m_wireData.get();
	case lcsm::model::Digit::Port::WiringDecimalPoint:
		return m_hasDecimalPoint ? m_wireDecimalPoint.get() : nullptr;
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Digit::findPort(const lcsm::Component *circuit) const noexcept
{
	if (circuit == m_wireData.get())
		return lcsm::model::Digit::Port::WiringData;
	else if (circuit == m_wireDecimalPoint.get() && m_hasDecimalPoint)
		return lcsm::model::Digit::Port::WiringDecimalPoint;
	else
		return -1;
}

lcsm::portid_t lcsm::model::Digit::defaultPort() const noexcept
{
	return lcsm::model::Digit::Port::WiringData;
}

void lcsm::model::Digit::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	// name <STRING>;
	writer.writeNameDeclaration(name());

	// keyvalue hasDecimalPoint <BOOLEAN>;
	writer.writeKeyValueDeclaration("hasDecimalPoint", hasDecimalPoint());

	// keyvalue wiredataid <INTEGER>;
	writer.writeKeyValueDeclaration("wiredataid", wireData()->id());

	// keyvalue wiredecimalpointid <INTEGER>;
	writer.writeKeyValueDeclaration("wiredecimalpointid", wireDecimalPoint()->id());

	builder.addWires(m_wireData.get(), true);

	if (m_hasDecimalPoint)
	{
		builder.addWires(m_wireDecimalPoint.get(), true);
	}
}

void lcsm::model::Digit::copy(lcsm::Component *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (componentType() != circuit->componentType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::Digit *digit = static_cast< lcsm::model::Digit * >(circuit);
	digit->setName(name());
	digit->setHasDecimalPoint(hasDecimalPoint());

	builder.oldToNew(id(), digit->id());
	builder.oldToNew(wireData()->id(), digit->wireData()->id());
	builder.oldToNew(wireDecimalPoint()->id(), digit->wireDecimalPoint()->id());

	builder.addWires(wireData(), true);
	builder.addWires(wireDecimalPoint(), true);
}

void lcsm::model::Digit::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// name <STRING>;
	setName(reader.exceptName());

	// keyvalue hasDecimalPoint BOOLEAN;
	setHasDecimalPoint(reader.exceptBooleanKeyValue("hasDecimalPoint"));

	// keyvalue wiredataid INTEGER;
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("wiredataid")), wireData()->id());

	// keyvalue wiredecimalpointid INTEGER:
	builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue("wiredecimalpointid")), wireDecimalPoint()->id());
}
