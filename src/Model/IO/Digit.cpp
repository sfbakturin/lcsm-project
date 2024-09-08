#include <sim/Component/Identifier.h>
#include <sim/Model/IO/Digit.h>

#include <stdexcept>
#include <utility>

sim::Digit::Digit(bool hasDecimalPoint) : m_hasDecimalPoint(hasDecimalPoint) {}

sim::Digit::Digit(const sim::Digit &other) :
	m_hasDecimalPoint(other.m_hasDecimalPoint)
{
}
sim::Digit::Digit(sim::Digit &&other) noexcept :
	m_hasDecimalPoint(other.m_hasDecimalPoint)
{
}

sim::Digit &sim::Digit::operator=(const sim::Digit &other)
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

sim::Identifier sim::Digit::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Digit::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_decimalPoint.identify(m_data.identify(m_id.next()));
}

void sim::Digit::ConnectIn(sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
		m_data.ConnectWire(wire);
	else if (i == 1)
		m_decimalPoint.ConnectWire(wire);
	else
		throw std::logic_error("Digit element has only 2 inputs.");
}
void sim::Digit::ConnectOut(sim::wire_t &, std::size_t)
{
	throw std::logic_error("Digit element doesn't have any outputs.");
}
