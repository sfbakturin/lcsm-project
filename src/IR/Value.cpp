#include <sim/IR/Bit.h>
#include <sim/IR/Value.h>

#include <cstddef>
#include <stdexcept>
#include <utility>

sim::Value::Value() noexcept : m_width(1)
{
	ResetBits();
}
sim::Value::Value(std::size_t width) noexcept : m_width(width)
{
	ResetBits();
}
sim::Value::Value(const sim::Value &other) : m_width(other.m_width), m_bits(other.m_bits) {}
sim::Value::Value(sim::Value &&other) noexcept : m_width(other.m_width), m_bits(std::move(other.m_bits)) {}

sim::Value &sim::Value::operator=(const sim::Value &other)
{
	if (this != &other)
		sim::Value(other).Swap(*this);
	return *this;
}
sim::Value &sim::Value::operator=(sim::Value &&other) noexcept
{
	if (this != &other)
		sim::Value(std::move(other)).Swap(*this);
	return *this;
}

void sim::Value::Swap(sim::Value &other) noexcept
{
	std::swap(m_width, other.m_width);
	std::swap(m_bits, other.m_bits);
}

std::size_t sim::Value::GetWidth() const noexcept
{
	return m_width;
}
bool sim::Value::IsError() const noexcept
{
	for (std::size_t i = 0; i < m_width; i++)
		if (m_bits[i] == sim::Bit::LOGISIM_ERROR)
			return true;
	return false;
}
bool sim::Value::IsOK() const noexcept
{
	return !IsError();
}

sim::Bit sim::Value::GetBit(std::size_t i) const
{
	if (i >= m_width)
		throw std::logic_error("Incompatible widths found.");
	return m_bits[i];
}

void sim::Value::ResetBits() noexcept
{
	for (std::size_t i = 0; i < m_width; i++)
		m_bits[i] = sim::Bit::LOGISIM_UNDEFINED;
}
