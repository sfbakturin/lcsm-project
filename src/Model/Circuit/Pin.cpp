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
		sim::Pin(other).Swap(*this);
	return *this;
}

sim::Pin &sim::Pin::operator=(sim::Pin &&other) noexcept
{
	if (this != &other)
		sim::Pin(std::move(other)).Swap(*this);
	return *this;
}

void sim::Pin::Swap(sim::Pin &other) noexcept
{
	std::swap(m_output, other.m_output);
	std::swap(m_width, other.m_width);
}

bool sim::Pin::IsOutput() const noexcept
{
	return m_output;
}

sim::Width sim::Pin::GetWidth() const noexcept
{
	return m_width;
}

sim::Wire &sim::Pin::GetWire() noexcept
{
	return m_wire;
}

const sim::Wire &sim::Pin::GetWire() const noexcept
{
	return m_wire;
}

void sim::Pin::SetOutput(bool newOutput) noexcept
{
	m_output = newOutput;
}

void sim::Pin::SetWidth(sim::Width newWidth) noexcept
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

void sim::Pin::ConnectIn(sim::wire_t &wire, std::size_t i)
{
	if (m_output)
		throw std::logic_error("Constant element doesn't have input.");
	else if (i != 0)
		throw std::logic_error("Constant element has only one input.");
	else
		m_wire.ConnectWire(wire);
	wire->ConnectWire(m_wire);
}

void sim::Pin::ConnectOut(sim::wire_t &wire, std::size_t i)
{
	if (!m_output)
		throw std::logic_error("Constant element doesn't have output.");
	else if (i != 0)
		throw std::logic_error("Constant element has only one output.");
	else
		m_wire.ConnectWire(wire);
	wire->ConnectWire(m_wire);
}

void sim::Pin::Connect(sim::wire_t &wire)
{
	ConnectOut(wire, 0);
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
