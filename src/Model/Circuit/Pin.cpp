#include <sim/Component/Identifier.h>
#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Pin.h>

#include <stdexcept>
#include <utility>

sim::Pin::Pin(bool output, sim::Width width) :
	m_output(output), m_width(width), m_wire(this)
{
}

sim::Pin::Pin(const sim::Pin &other) :
	m_output(other.m_output), m_width(other.m_width)
{
}
sim::Pin::Pin(sim::Pin &&other) noexcept :
	m_output(other.m_output), m_width(other.m_width)
{
}

sim::Pin &sim::Pin::operator=(const sim::Pin &other)
{
	if (this != &other)
		sim::Pin(other).swap(*this);
	return *this;
}

sim::Pin &sim::Pin::operator=(sim::Pin &&other) noexcept
{
	if (this != &other)
		sim::Pin(std::move(other)).swap(*this);
	return *this;
}

void sim::Pin::swap(sim::Pin &other) noexcept
{
	std::swap(m_output, other.m_output);
	std::swap(m_width, other.m_width);
}

bool sim::Pin::isOutput() const noexcept
{
	return m_output;
}

sim::Width sim::Pin::width() const noexcept
{
	return m_width;
}

sim::Wire &sim::Pin::wire() noexcept
{
	return m_wire;
}

const sim::Wire &sim::Pin::wire() const noexcept
{
	return m_wire;
}

void sim::Pin::setOutput(bool newOutput) noexcept
{
	m_output = newOutput;
}

void sim::Pin::setWidth(sim::Width newWidth) noexcept
{
	m_width = newWidth;
}

sim::Identifier sim::Pin::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Pin::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wire.identify(m_id.next());
}

void sim::Pin::connectIn(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Constant element has only one input.");
	else
		m_wire.connectWire(wire);
	wire->connectWire(m_wire);
}

void sim::Pin::connectOut(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Constant element has only one output.");
	else
		m_wire.connectWire(wire);
	wire->connectWire(m_wire);
}

void sim::Pin::connect(sim::wire_t &wire)
{
	connectOut(wire, 0);
}

sim::CircuitComponentType sim::Pin::circuitComponentType() const noexcept
{
	return sim::CircuitComponentType::CIRCUIT_COMP_PIN;
}

sim::Pin *sim::Pin::asPin() noexcept
{
	return this;
}

const sim::Pin *sim::Pin::asPin() const noexcept
{
	return this;
}
