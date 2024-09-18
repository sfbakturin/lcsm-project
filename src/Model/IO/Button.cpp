#include <sim/Component/IOComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Model/IO/Button.h>
#include <sim/Model/Wiring/Wire.h>

#include <stdexcept>
#include <utility>

sim::Button::Button(bool activeOnPress) :
	m_activeOnPress(activeOnPress), m_wireOut(this)
{
}

sim::Button::Button(const sim::Button &other) :
	m_activeOnPress(other.m_activeOnPress)
{
}

sim::Button::Button(sim::Button &&other) noexcept :
	m_activeOnPress(other.m_activeOnPress)
{
}

sim::Button &sim::Button::operator=(const sim::Button &other)
{
	if (this != &other)
		sim::Button(other).swap(*this);
	return *this;
}

sim::Button &sim::Button::operator=(sim::Button &&other) noexcept
{
	if (this != &other)
		sim::Button(std::move(other)).swap(*this);
	return *this;
}

void sim::Button::swap(sim::Button &other) noexcept
{
	std::swap(m_activeOnPress, other.m_activeOnPress);
}

sim::Identifier sim::Button::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Button::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wireOut.identify(m_id.next());
}

void sim::Button::connectIn(sim::wire_t &, std::size_t)
{
	throw std::logic_error("Button element doesn't have any inputs.");
}

void sim::Button::connectOut(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Button element has only 1 output.");
	m_wireOut.connectWire(wire);
	wire->connectWire(m_wireOut);
}

bool sim::Button::activeOnPress() const noexcept
{
	return m_activeOnPress;
}

void sim::Button::setActiveOnPress(bool activeOnPress) noexcept
{
	m_activeOnPress = activeOnPress;
}

sim::IOComponentType sim::Button::ioComponentType() const noexcept
{
	return sim::IOComponentType::IO_COMP_BUTTON;
}
