#include <initializer_list>
#include <lcsm/IR/Bit.h>
#include <lcsm/IR/Value.h>
#include <lcsm/IR/Width.h>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

static unsigned roundPow2(unsigned n) noexcept
{
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;
	return n;
}

lcsm::Value::Value() noexcept : m_width(lcsm::Width::W1)
{
	resetBits();
}

lcsm::Value::Value(lcsm::Width width) noexcept : m_width(width)
{
	resetBits();
}

lcsm::Value::Value(lcsm::Width width, lcsm::LogisimBit bit) noexcept : m_width(width)
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = bit;
}

lcsm::Value::Value(std::initializer_list< lcsm::LogisimBit > value)
{
	resetBits();

	const unsigned n = roundPow2(value.size());

	if (n > lcsm::Width::W64)
	{
		throw std::logic_error("");
	}

	m_width = static_cast< lcsm::Width >(n);
	std::size_t i = 0;

	for (std::initializer_list< lcsm::LogisimBit >::iterator it = value.begin(); it < value.end(); it++)
	{
		m_bits[i++] = *it;
	}
}

lcsm::Value::Value(const lcsm::Value &other) : m_width(other.m_width)
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = other.m_bits[i];
}

lcsm::Value::Value(lcsm::Value &&other) noexcept : m_width(other.m_width)
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = other.m_bits[i];
}

lcsm::Value &lcsm::Value::operator=(const lcsm::Value &other)
{
	if (this != std::addressof(other))
		lcsm::Value(other).swap(*this);
	return *this;
}

lcsm::Value &lcsm::Value::operator=(lcsm::Value &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Value(std::move(other)).swap(*this);
	return *this;
}

bool lcsm::Value::operator==(const lcsm::Value &other)
{
	if (width() != other.width())
	{
		return false;
	}

	const lcsm::Width width = other.width();
	for (unsigned i = 0; i < width; i++)
		if (m_bits[i] != other.bit(i))
			return false;

	return true;
}

bool lcsm::Value::operator!=(const lcsm::Value &other)
{
	return !operator==(other);
}

void lcsm::Value::swap(lcsm::Value &other) noexcept
{
	std::swap(m_width, other.m_width);
	std::swap(m_bits, other.m_bits);
}

lcsm::Width lcsm::Value::width() const noexcept
{
	return m_width;
}

void lcsm::Value::setWidth(lcsm::Width newWidth) noexcept
{
	m_width = newWidth;
}

bool lcsm::Value::isError() const noexcept
{
	for (std::size_t i = 0; i < m_width; i++)
		if (m_bits[i] == lcsm::LogisimBit::LOGISIM_ERROR)
			return true;
	return false;
}

bool lcsm::Value::isOK() const noexcept
{
	return !isError();
}

lcsm::LogisimBit lcsm::Value::bit(std::size_t i) const
{
	if (i >= m_width)
		throw std::logic_error("Incompatible widths found.");
	return m_bits[i];
}

void lcsm::Value::setBit(std::size_t i, lcsm::LogisimBit b)
{
	if (i >= m_width)
		throw std::logic_error("Incompatible widths found.");
	m_bits[i] = b;
}

void lcsm::Value::resetBits() noexcept
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = lcsm::LogisimBit::LOGISIM_UNDEFINED;
}
