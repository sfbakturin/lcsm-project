#include <sim/Component/IOComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Model/IO/Digit.h>
#include <sim/Model/Wiring/Wire.h>

#include <memory>
#include <stdexcept>
#include <utility>

sim::Digit::Digit(bool hasDecimalPoint) :
	m_hasDecimalPoint(hasDecimalPoint), m_data(this), m_decimalPoint(this)
{
}

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
		sim::Digit(other).swap(*this);
	return *this;
}
sim::Digit &sim::Digit::operator=(sim::Digit &other) noexcept
{
	if (this != &other)
		sim::Digit(std::move(other)).swap(*this);
	return *this;
}

void sim::Digit::swap(sim::Digit &other) noexcept
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

void sim::Digit::connectIn(sim::wire_t &wire, std::size_t i)
{
	sim::Wire *selected = nullptr;

	switch (i)
	{
	case sim::Digit::CompositeIndex::DIGIT_INDEX_DATA:
		selected = std::addressof(m_data);
		break;
	case sim::Digit::CompositeIndex::DIGIT_INDEX_DECIMAL_POINT:
		selected = std::addressof(m_decimalPoint);
		break;
	default:
		throw std::logic_error(
			"Digit element has only data and decimal point "
			"element");
	}

	selected->connectWire(wire);
	wire->connectWire(selected);
}

void sim::Digit::connectOut(sim::wire_t &, std::size_t)
{
	throw std::logic_error("Digit element doesn't have any outputs.");
}

bool sim::Digit::hasDecimalPoint() const noexcept
{
	return m_hasDecimalPoint;
}

void sim::Digit::setHasDecimalPoint(bool hasDecimalPoint) noexcept
{
	m_hasDecimalPoint = hasDecimalPoint;
}

sim::IOComponentType sim::Digit::ioComponentType() const noexcept
{
	return sim::IOComponentType::IO_COMP_DIGIT;
}
