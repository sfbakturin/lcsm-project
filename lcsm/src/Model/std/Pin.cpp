#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Pin::Pin(bool output, lcsm::Width width) : m_output(output), m_width(width)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_internal.connectConnect(circuit);
	m_external.connectConnect(circuit);
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

const lcsm::model::Wire &lcsm::model::Pin::internal() const noexcept
{
	return m_internal;
}

const lcsm::model::Wire &lcsm::model::Pin::external() const noexcept
{
	return m_external;
}

lcsm::Identifier lcsm::model::Pin::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Pin::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	const lcsm::Identifier next = m_internal.identify(m_id.next());
	return m_external.identify(next);
}

lcsm::ObjectType lcsm::model::Pin::objectType() const noexcept
{
	return (m_output ? lcsm::ObjectType::IntExtOut : lcsm::ObjectType::IntExtIn);
}

lcsm::CircuitType lcsm::model::Pin::circuitType() const noexcept
{
	return lcsm::CircuitType::Pin;
}

void lcsm::model::Pin::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Pin::Port pp = static_cast< lcsm::model::Pin::Port >(portId);
	switch (pp)
	{
	case lcsm::model::Pin::Port::Internal:
	{
		m_internal.connectToWire(circuit);
		break;
	}
	case lcsm::model::Pin::Port::External:
	{
		m_external.connectToWire(circuit);
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Pin::connectInternal(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Pin::Port::Internal);
	connect(portId, circuit);
}

void lcsm::model::Pin::connectExternal(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Pin::Port::External);
	connect(portId, circuit);
}

lcsm::Circuit *lcsm::model::Pin::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Pin::Port p = static_cast< lcsm::model::Pin::Port >(portId);
	switch (p)
	{
	case lcsm::model::Pin::Port::Internal:
		return std::addressof(m_internal);
	case lcsm::model::Pin::Port::External:
		return std::addressof(m_external);
	}
	return nullptr;
}
