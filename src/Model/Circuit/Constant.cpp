#include <sim/Component/Identifier.h>
#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Constant.h>

#include <stdexcept>
#include <utility>

sim::Constant::Constant(sim::Width width, std::uint64_t value) :
	m_width(width), m_value(value), m_wireOut(this)
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
		Constant(other).swap(*this);
	return *this;
}

sim::Constant &sim::Constant::operator=(sim::Constant &&other) noexcept
{
	if (this != &other)
		Constant(std::move(other)).swap(*this);
	return *this;
}

void sim::Constant::swap(sim::Constant &other) noexcept
{
	std::swap(m_value, other.m_value);
	std::swap(m_width, other.m_width);
}

sim::Width sim::Constant::width() const noexcept
{
	return m_width;
}

std::uint64_t sim::Constant::value() const noexcept
{
	return m_value;
}

void sim::Constant::setWidth(sim::Width newWidth) noexcept
{
	m_width = newWidth;
}

void sim::Constant::setValue(std::uint64_t newValue) noexcept
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

sim::Identifier sim::Constant::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Constant::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wireOut.identify(m_id.next());
}

void sim::Constant::connectIn(sim::wire_t &, std::size_t)
{
	throw std::logic_error("Constant element doesn't have input.");
}

void sim::Constant::connectOut(sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
	{
		m_wireOut.connectWire(wire);
		wire->connectWire(m_wireOut);
	}
	else
	{
		throw std::logic_error("Constant element has only one output.");
	}
}

void sim::Constant::connect(sim::wire_t &wire)
{
	connectOut(wire, 0);
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
