#include <lcsm/Support/Numbers.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <memory>
#include <ostream>
#include <utility>

lcsm::verilog::Value::Value() noexcept : Value(lcsm::verilog::Strength::StrongDrive, lcsm::verilog::Bit::False) {}

lcsm::verilog::Value::Value(lcsm::verilog::Bit bit) noexcept : Value(lcsm::verilog::Strength::StrongDrive, bit) {}

lcsm::verilog::Value::Value(lcsm::verilog::Strength strength, lcsm::verilog::Bit bit) noexcept :
	m_strength(strength), m_bit(bit)
{
}

lcsm::verilog::Value::Value(const lcsm::verilog::Value &other) noexcept :
	m_strength(other.m_strength), m_bit(other.m_bit)
{
}

lcsm::verilog::Value::Value(lcsm::verilog::Value &&other) noexcept : m_strength(other.m_strength), m_bit(other.m_bit) {}

lcsm::verilog::Value &lcsm::verilog::Value::operator=(const lcsm::verilog::Value &other) noexcept
{
	if (this != std::addressof(other))
		lcsm::verilog::Value(other).swap(*this);
	return *this;
}

lcsm::verilog::Value &lcsm::verilog::Value::operator=(lcsm::verilog::Value &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::verilog::Value(std::move(other)).swap(*this);
	return *this;
}

lcsm::verilog::Value lcsm::verilog::Value::operator|(const lcsm::verilog::Value &other) noexcept
{
	/* If values equals strictly, then return left. */
	if (*this == other)
		return *this;

	/* If anyone is undefined and it's more strengths, than it's already undefined. */
	if (bit() == lcsm::verilog::Bit::Undefined && strength() > other.strength())
		return { strength(), bit() };
	else if (other.bit() == lcsm::verilog::Bit::Undefined && strength() < other.strength())
		return { other.strength(), other.bit() };

	/* If left less-outer (by strength, bit's equals) than right, then result should have right's strength. */
	if (lessOuter(other))
		return other;
	/* If left greater-other (by strength bit's equals) than right, then result should have left's strength. */
	else if (greaterOuter(other))
		return *this;
	/* If left less than right by strength, then result should be right. */
	else if (strength() < other.strength())
		return other;
	/* If left greater than right by strength, then result should be left. */
	else if (strength() > other.strength())
		return *this;
	/* Otherwise, this is undefined (Verilog's X-bit). */
	else
		return { other.strength(), lcsm::verilog::Bit::Undefined };
}

lcsm::verilog::Value &lcsm::verilog::Value::operator|=(const lcsm::verilog::Value &other) noexcept
{
	if (this != std::addressof(other))
		*this = *this | other;
	return *this;
}

bool lcsm::verilog::Value::operator==(const lcsm::verilog::Value &other) const noexcept
{
	return equalsInner(other) && strength() == other.strength();
}

bool lcsm::verilog::Value::operator!=(const lcsm::verilog::Value &other) const noexcept
{
	return !(*this == other);
}

int lcsm::verilog::Value::compareInner(const lcsm::verilog::Value &left, const lcsm::verilog::Value &right) noexcept
{
	return lcsm::support::compare(left.bit(), right.bit());
}

int lcsm::verilog::Value::compareOuter(const lcsm::verilog::Value &left, const lcsm::verilog::Value &right) noexcept
{
	return (left.equalsInner(right) ? 0 : lcsm::support::compare(left.strength(), right.strength()));
}

bool lcsm::verilog::Value::lessInner(const lcsm::verilog::Value &other) const noexcept
{
	return lcsm::verilog::Value::compareInner(*this, other) == 1;
}

bool lcsm::verilog::Value::greaterInner(const lcsm::verilog::Value &other) const noexcept
{
	return lcsm::verilog::Value::compareInner(*this, other) == -1;
}

bool lcsm::verilog::Value::equalsInner(const lcsm::verilog::Value &other) const noexcept
{
	return lcsm::verilog::Value::compareInner(*this, other) == 0;
}

bool lcsm::verilog::Value::lessOuter(const lcsm::verilog::Value &other) const noexcept
{
	return lcsm::verilog::Value::compareOuter(*this, other) == 1;
}

bool lcsm::verilog::Value::greaterOuter(const lcsm::verilog::Value &other) const noexcept
{
	return lcsm::verilog::Value::compareOuter(*this, other) == -1;
}

bool lcsm::verilog::Value::equalsOuter(const lcsm::verilog::Value &other) const noexcept
{
	return lcsm::verilog::Value::compareOuter(*this, other) == 0;
}

void lcsm::verilog::Value::swap(lcsm::verilog::Value &other) noexcept
{
	std::swap(m_strength, other.m_strength);
	std::swap(m_bit, other.m_bit);
}

lcsm::verilog::Value lcsm::verilog::Value::one(lcsm::verilog::Strength strength) noexcept
{
	return lcsm::verilog::Value(strength, lcsm::verilog::Bit::True);
}

lcsm::verilog::Value lcsm::verilog::Value::zero(lcsm::verilog::Strength strength) noexcept
{
	return lcsm::verilog::Value(strength, lcsm::verilog::Bit::False);
}

lcsm::verilog::Strength lcsm::verilog::Value::strength() const noexcept
{
	return m_strength;
}

void lcsm::verilog::Value::setStrength(lcsm::verilog::Strength newStrength) noexcept
{
	m_strength = newStrength;
}

lcsm::verilog::Bit lcsm::verilog::Value::bit() const noexcept
{
	return m_bit;
}

void lcsm::verilog::Value::setBit(lcsm::verilog::Bit newBit) noexcept
{
	m_bit = newBit;
}

void lcsm::verilog::Value::flip() noexcept
{
	if (m_bit == lcsm::verilog::Bit::True)
		setBit(lcsm::verilog::Bit::False);
	else if (m_bit == lcsm::verilog::Bit::False)
		setBit(lcsm::verilog::Bit::True);
}

std::ostream &lcsm::verilog::operator<<(std::ostream &os, const Value &v)
{
	return os << '{' << v.m_bit << ", " << v.m_strength << '}';
}
