#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Constant.h>

#include <stdexcept>
#include <utility>

sim::Constant::Constant(sim::Width width, std::uint64_t value) :
	m_id(0), m_width(width), m_value(value)
{
}

sim::Constant::Constant(const sim::Constant &other) :
	m_width(other.m_width), m_value(other.m_value)
{
}

sim::Constant::Constant(sim::Constant &&other) noexcept :
	m_width(other.m_width), m_value(other.m_value)
{
}

sim::Constant &sim::Constant::operator=(const sim::Constant &other)
{
	if (this != &other)
	{
		Constant(other).Swap(*this);
	}

	return *this;
}

sim::Constant &sim::Constant::operator=(sim::Constant &&other) noexcept
{
	if (this != &other)
	{
		Constant(std::move(other)).Swap(*this);
	}

	return *this;
}

void sim::Constant::Swap(sim::Constant &other) noexcept
{
	std::swap(m_value, other.m_value);
	std::swap(m_width, other.m_width);
}

sim::Width sim::Constant::GetWidth() const noexcept
{
	return m_width;
}

std::uint64_t sim::Constant::GetValue() const noexcept
{
	return m_value;
}

void sim::Constant::SetWidth(sim::Width newWidth) noexcept
{
	m_width = newWidth;
}

void sim::Constant::SetValue(std::uint64_t newValue) noexcept
{
	m_value = newValue;
}

sim::Wire &sim::Constant::wire() noexcept
{
	return m_wireOut;
}

const sim::Wire &sim::Constant::wire() const noexcept
{
	return m_wireOut;
}

unsigned sim::Constant::ID() const noexcept
{
	return m_id;
}

void sim::Constant::Identify(unsigned ID) noexcept
{
	m_id = ID;
}

void sim::Constant::ConnectIn(sim::wire_t &, std::size_t)
{
	throw std::logic_error("Constant element doesn't have input.");
}

void sim::Constant::ConnectOut(sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
		m_wireOut.ConnectWire(wire);
	throw std::logic_error("Constant element has only one output.");
}

void sim::Constant::Connect(sim::wire_t &wire)
{
	ConnectOut(wire, 0);
}

sim::CircuitComponentType sim::Constant::circuitComponentType() const noexcept
{
	return sim::CircuitComponentType::CIRCUIT_COMP_CONSTANT;
}

sim::Constant *sim::Constant::asConstant() noexcept
{
	return this;
}

const sim::Constant *sim::Constant::asConstant() const noexcept
{
	return this;
}
