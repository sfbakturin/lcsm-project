#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Pin.h>

#include <stdexcept>
#include <utility>

sim::Pin::Pin(bool output, sim::Width width, std::uint64_t value) : m_output(output), m_width(width), m_value(value) {}

sim::Pin::Pin(const sim::Pin &other) : m_output(other.m_output), m_width(other.m_width), m_value(other.m_value) {}
sim::Pin::Pin(sim::Pin &&other) noexcept : m_output(other.m_output), m_width(other.m_width), m_value(other.m_value) {}

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
	std::swap(m_value, other.m_value);
}

sim::Width sim::Pin::GetWidth() const noexcept
{
	return m_width;
}
std::uint64_t sim::Pin::GetValue() const noexcept
{
	return m_value;
}

void sim::Pin::SetWidth(sim::Width newWidth) noexcept
{
	m_width = newWidth;
}
void sim::Pin::SetValue(std::uint64_t newValue) noexcept
{
	m_value = newValue;
}

void sim::Pin::ConnectIn(const sim::wire_t &wire, std::size_t i)
{
	if (m_output)
		throw std::logic_error("Constant element doesn't have input.");
	else if (i != 0)
		throw std::logic_error("Constant element has only one input.");
	else
		m_wire.ConnectWire(wire);
}

void sim::Pin::ConnectOut(const sim::wire_t &wire, std::size_t i)
{
	if (!m_output)
		throw std::logic_error("Constant element doesn't have output.");
	else if (i != 0)
		throw std::logic_error("Constant element has only one output.");
	else
		m_wire.ConnectWire(wire);
}

void sim::Pin::Connect(const sim::wire_t &wire)
{
	ConnectOut(wire, 0);
}
