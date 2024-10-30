#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Circuit/Constant.h>
#include <lcsm/Model/Width.h>

#include <stdexcept>
#include <utility>

lcsm::model::Constant::Constant(lcsm::Width width, std::uint64_t value) :
	m_width(width), m_value(value), m_wireOut(this)
{
}

lcsm::model::Constant::Constant(const lcsm::model::Constant &other) : m_width(other.m_width), m_value(other.m_value) {}

lcsm::model::Constant::Constant(lcsm::model::Constant &&other) noexcept : m_width(other.m_width), m_value(other.m_value)
{
}

lcsm::model::Constant &lcsm::model::Constant::operator=(const lcsm::model::Constant &other)
{
	if (this != &other)
		Constant(other).swap(*this);
	return *this;
}

lcsm::model::Constant &lcsm::model::Constant::operator=(lcsm::model::Constant &&other) noexcept
{
	if (this != &other)
		Constant(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Constant::swap(lcsm::model::Constant &other) noexcept
{
	std::swap(m_value, other.m_value);
	std::swap(m_width, other.m_width);
}

lcsm::Width lcsm::model::Constant::width() const noexcept
{
	return m_width;
}

std::uint64_t lcsm::model::Constant::value() const noexcept
{
	return m_value;
}

void lcsm::model::Constant::setWidth(lcsm::Width newWidth) noexcept
{
	m_width = newWidth;
}

void lcsm::model::Constant::setValue(std::uint64_t newValue) noexcept
{
	m_value = newValue;
}

lcsm::model::Wire &lcsm::model::Constant::wire() noexcept
{
	return m_wireOut;
}

const lcsm::model::Wire &lcsm::model::Constant::wire() const noexcept
{
	return m_wireOut;
}

lcsm::Identifier lcsm::model::Constant::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Constant::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wireOut.identify(m_id.next());
}

void lcsm::model::Constant::connectIn(lcsm::wire_t &, std::size_t)
{
	throw std::logic_error("Constant element doesn't have input.");
}

void lcsm::model::Constant::connectOut(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Constant element has only one output.");

	m_wireOut.connectWire(wire);
	wire->connectWire(m_wireOut);
}

void lcsm::model::Constant::connect(lcsm::wire_t &wire)
{
	connectOut(wire, 0);
}

lcsm::CircuitComponentType lcsm::model::Constant::circuitComponentType() const noexcept
{
	return lcsm::CircuitComponentType::CIRCUIT_COMP_CONSTANT;
}

lcsm::model::Constant *lcsm::model::Constant::asConstant() noexcept
{
	return this;
}

const lcsm::model::Constant *lcsm::model::Constant::asConstant() const noexcept
{
	return this;
}
