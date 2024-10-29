#include <lcsm/Component/Identifier.h>
#include <lcsm/IR/Width.h>
#include <lcsm/Model/Circuit/Pin.h>

#include <stdexcept>
#include <utility>

lcsm::model::Pin::Pin(bool output, lcsm::Width width) : m_output(output), m_width(width), m_wire(this) {}

lcsm::model::Pin::Pin(const lcsm::model::Pin &other) : m_output(other.m_output), m_width(other.m_width) {}
lcsm::model::Pin::Pin(lcsm::model::Pin &&other) noexcept : m_output(other.m_output), m_width(other.m_width) {}

lcsm::model::Pin &lcsm::model::Pin::operator=(const lcsm::model::Pin &other)
{
	if (this != &other)
		lcsm::model::Pin(other).swap(*this);
	return *this;
}

lcsm::model::Pin &lcsm::model::Pin::operator=(lcsm::model::Pin &&other) noexcept
{
	if (this != &other)
		lcsm::model::Pin(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Pin::swap(lcsm::model::Pin &other) noexcept
{
	std::swap(m_output, other.m_output);
	std::swap(m_width, other.m_width);
}

bool lcsm::model::Pin::isOutput() const noexcept
{
	return m_output;
}

lcsm::Width lcsm::model::Pin::width() const noexcept
{
	return m_width;
}

lcsm::model::Wire &lcsm::model::Pin::wire() noexcept
{
	return m_wire;
}

const lcsm::model::Wire &lcsm::model::Pin::wire() const noexcept
{
	return m_wire;
}

void lcsm::model::Pin::setOutput(bool newOutput) noexcept
{
	m_output = newOutput;
}

void lcsm::model::Pin::setWidth(lcsm::Width newWidth) noexcept
{
	m_width = newWidth;
}

lcsm::Identifier lcsm::model::Pin::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Pin::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wire.identify(m_id.next());
}

void lcsm::model::Pin::connectIn(lcsm::wire_t &wire, std::size_t i)
{
	connectOut(wire, i);
}

void lcsm::model::Pin::connectOut(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Constant element has only one input/output.");
	m_wire.connectWire(wire);
	wire->connectWire(m_wire);
}

void lcsm::model::Pin::connect(lcsm::wire_t &wire)
{
	connectOut(wire, 0);
}

lcsm::CircuitComponentType lcsm::model::Pin::circuitComponentType() const noexcept
{
	return lcsm::CircuitComponentType::CIRCUIT_COMP_PIN;
}

lcsm::model::Pin *lcsm::model::Pin::asPin() noexcept
{
	return this;
}

const lcsm::model::Pin *lcsm::model::Pin::asPin() const noexcept
{
	return this;
}
