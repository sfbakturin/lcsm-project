#include <initializer_list>
#include <sim/IR/Bit.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

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

sim::Value::Value() noexcept : m_width(sim::Width::W1)
{
	resetBits();
}

sim::Value::Value(sim::Width width) noexcept : m_width(width)
{
	resetBits();
}

sim::Value::Value(sim::Width width, sim::LogisimBit bit) noexcept :
	m_width(width)
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = bit;
}

sim::Value::Value(std::initializer_list< sim::LogisimBit > value)
{
	resetBits();

	const unsigned n = roundPow2(value.size());

	if (n > sim::Width::W64)
	{
		throw std::logic_error("");
	}

	m_width = static_cast< sim::Width >(n);
	std::size_t i = 0;

	for (std::initializer_list< sim::LogisimBit >::iterator it = value.begin();
		 it < value.end();
		 it++)
	{
		m_bits[i++] = *it;
	}
}

sim::Value::Value(const sim::Value &other) : m_width(other.m_width)
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = other.m_bits[i];
}

sim::Value::Value(sim::Value &&other) noexcept : m_width(other.m_width)
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = other.m_bits[i];
}

sim::Value &sim::Value::operator=(const sim::Value &other)
{
	if (this != std::addressof(other))
		sim::Value(other).swap(*this);
	return *this;
}

sim::Value &sim::Value::operator=(sim::Value &&other) noexcept
{
	if (this != std::addressof(other))
		sim::Value(std::move(other)).swap(*this);
	return *this;
}

bool sim::Value::operator==(const sim::Value &other)
{
	if (width() != other.width())
	{
		return false;
	}

	const sim::Width width = other.width();
	for (unsigned i = 0; i < width; i++)
		if (m_bits[i] != other.bit(i))
			return false;

	return true;
}

bool sim::Value::operator!=(const sim::Value &other)
{
	return !operator==(other);
}

void sim::Value::swap(sim::Value &other) noexcept
{
	std::swap(m_width, other.m_width);
	std::swap(m_bits, other.m_bits);
}

sim::Width sim::Value::width() const noexcept
{
	return m_width;
}

void sim::Value::setWidth(sim::Width newWidth) noexcept
{
	m_width = newWidth;
}

bool sim::Value::isError() const noexcept
{
	for (std::size_t i = 0; i < m_width; i++)
		if (m_bits[i] == sim::LogisimBit::LOGISIM_ERROR)
			return true;
	return false;
}

bool sim::Value::isOK() const noexcept
{
	return !isError();
}

sim::LogisimBit sim::Value::bit(std::size_t i) const
{
	if (i >= m_width)
		throw std::logic_error("Incompatible widths found.");
	return m_bits[i];
}

void sim::Value::setBit(std::size_t i, sim::LogisimBit b)
{
	if (i >= m_width)
		throw std::logic_error("Incompatible widths found.");
	m_bits[i] = b;
}

void sim::Value::resetBits() noexcept
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = sim::LogisimBit::LOGISIM_UNDEFINED;
}
