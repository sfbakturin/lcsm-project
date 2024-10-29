#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/IO/Button.h>
#include <lcsm/Model/Wiring/Wire.h>

#include <stdexcept>
#include <utility>

lcsm::model::Button::Button(bool activeOnPress) : m_activeOnPress(activeOnPress), m_wireOut(this) {}

lcsm::model::Button::Button(const lcsm::model::Button &other) : m_activeOnPress(other.m_activeOnPress) {}

lcsm::model::Button::Button(lcsm::model::Button &&other) noexcept : m_activeOnPress(other.m_activeOnPress) {}

lcsm::model::Button &lcsm::model::Button::operator=(const lcsm::model::Button &other)
{
	if (this != &other)
		lcsm::model::Button(other).swap(*this);
	return *this;
}

lcsm::model::Button &lcsm::model::Button::operator=(lcsm::model::Button &&other) noexcept
{
	if (this != &other)
		lcsm::model::Button(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Button::swap(lcsm::model::Button &other) noexcept
{
	std::swap(m_activeOnPress, other.m_activeOnPress);
}

lcsm::Identifier lcsm::model::Button::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Button::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wireOut.identify(m_id.next());
}

void lcsm::model::Button::connectIn(lcsm::wire_t &, std::size_t)
{
	throw std::logic_error("Button element doesn't have any inputs.");
}

void lcsm::model::Button::connectOut(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Button element has only 1 output.");
	m_wireOut.connectWire(wire);
	wire->connectWire(m_wireOut);
}

bool lcsm::model::Button::activeOnPress() const noexcept
{
	return m_activeOnPress;
}

void lcsm::model::Button::setActiveOnPress(bool activeOnPress) noexcept
{
	m_activeOnPress = activeOnPress;
}

lcsm::IOComponentType lcsm::model::Button::ioComponentType() const noexcept
{
	return lcsm::IOComponentType::IO_COMP_BUTTON;
}
