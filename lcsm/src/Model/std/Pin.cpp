#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Pin::Pin(bool output, lcsm::Width width) : m_output(output), m_width(width) {}

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
	return lcsm::ObjectType::Internal | lcsm::ObjectType::External | (m_output ? lcsm::ObjectType::Output : lcsm::ObjectType::Input);
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

void lcsm::model::Pin::disconnect(lcsm::Circuit *)
{
	// Do nothing.
}

void lcsm::model::Pin::disconnectAll()
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
