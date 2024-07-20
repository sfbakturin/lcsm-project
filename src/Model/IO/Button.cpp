#include <sim/Model/IO/Button.h>

#include <stdexcept>
#include <utility>

sim::Button::Button() noexcept : m_activeOnPress(true) {}
sim::Button::Button(bool activeOnPress) noexcept : m_activeOnPress(activeOnPress) {}

sim::Button::Button(const sim::Button &other) noexcept : m_activeOnPress(other.m_activeOnPress) {}
sim::Button::Button(sim::Button &&other) noexcept : m_activeOnPress(other.m_activeOnPress) {}

sim::Button &sim::Button::operator=(const sim::Button &other) noexcept
{
	if (this != &other)
		sim::Button(other).Swap(*this);
	return *this;
}
sim::Button &sim::Button::operator=(sim::Button &&other) noexcept
{
	if (this != &other)
		sim::Button(std::move(other)).Swap(*this);
	return *this;
}

void sim::Button::Swap(sim::Button &other) noexcept
{
	std::swap(m_activeOnPress, other.m_activeOnPress);
}

void sim::Button::ConnectIn(const sim::wire_t &, std::size_t)
{
	throw std::logic_error("Button element doesn't have any inputs.");
}
void sim::Button::ConnectOut(const sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Button element has only 1 output.");
	m_wireOut.Val().ConnectWire(wire);
}
