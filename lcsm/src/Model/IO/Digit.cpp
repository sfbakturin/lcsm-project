#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/IO/Digit.h>
#include <lcsm/Model/Wiring/Wire.h>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Digit::Digit(bool hasDecimalPoint) : m_hasDecimalPoint(hasDecimalPoint), m_data(this), m_decimalPoint(this)
{
}

lcsm::model::Digit::Digit(const lcsm::model::Digit &other) : m_hasDecimalPoint(other.m_hasDecimalPoint) {}
lcsm::model::Digit::Digit(lcsm::model::Digit &&other) noexcept : m_hasDecimalPoint(other.m_hasDecimalPoint) {}

lcsm::model::Digit &lcsm::model::Digit::operator=(const lcsm::model::Digit &other)
{
	if (this != &other)
		lcsm::model::Digit(other).swap(*this);
	return *this;
}
lcsm::model::Digit &lcsm::model::Digit::operator=(lcsm::model::Digit &other) noexcept
{
	if (this != &other)
		lcsm::model::Digit(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Digit::swap(lcsm::model::Digit &other) noexcept
{
	std::swap(m_hasDecimalPoint, other.m_hasDecimalPoint);
}

lcsm::Identifier lcsm::model::Digit::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Digit::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_decimalPoint.identify(m_data.identify(m_id.next()));
}

void lcsm::model::Digit::connectIn(lcsm::wire_t &wire, std::size_t i)
{
	lcsm::model::Wire *selected = nullptr;

	switch (i)
	{
	case lcsm::model::Digit::CompositeIndex::DIGIT_INDEX_DATA:
		selected = std::addressof(m_data);
		break;
	case lcsm::model::Digit::CompositeIndex::DIGIT_INDEX_DECIMAL_POINT:
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

void lcsm::model::Digit::connectOut(lcsm::wire_t &, std::size_t)
{
	throw std::logic_error("Digit element doesn't have any outputs.");
}

bool lcsm::model::Digit::hasDecimalPoint() const noexcept
{
	return m_hasDecimalPoint;
}

void lcsm::model::Digit::setHasDecimalPoint(bool hasDecimalPoint) noexcept
{
	m_hasDecimalPoint = hasDecimalPoint;
}

lcsm::IOComponentType lcsm::model::Digit::ioComponentType() const noexcept
{
	return lcsm::IOComponentType::IO_COMP_DIGIT;
}
