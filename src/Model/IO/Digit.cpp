#include <sim/Model/IO/Digit.h>

#include <stdexcept>
#include <utility>

sim::Digit::Digit() noexcept : m_hasDecimalPoint(true) {}
sim::Digit::Digit(bool hasDecimalPoint) noexcept : m_hasDecimalPoint(hasDecimalPoint) {}

sim::Digit::Digit(const sim::Digit &other) noexcept : m_hasDecimalPoint(other.m_hasDecimalPoint) {}
sim::Digit::Digit(sim::Digit &&other) noexcept : m_hasDecimalPoint(other.m_hasDecimalPoint) {}

sim::Digit &sim::Digit::operator=(const sim::Digit &other) noexcept
{
	if (this != &other)
		sim::Digit(other).Swap(*this);
	return *this;
}
sim::Digit &sim::Digit::operator=(sim::Digit &other) noexcept
{
	if (this != &other)
		sim::Digit(std::move(other)).Swap(*this);
	return *this;
}

void sim::Digit::Swap(sim::Digit &other) noexcept
{
	std::swap(m_hasDecimalPoint, other.m_hasDecimalPoint);
}

void sim::Digit::ConnectIn(const sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
		m_data.Val().ConnectWire(wire);
	else if (i == 1)
		m_decimalPoint.Val().ConnectWire(wire);
	else
		throw std::logic_error("Digit element has only 2 inputs.");
}
void sim::Digit::ConnectOut(const sim::wire_t &wire, std::size_t i)
{
	throw std::logic_error("Digit element doesn't have any outputs.");
}
