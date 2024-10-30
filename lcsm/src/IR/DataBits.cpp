#include <initializer_list>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

lcsm::DataBits::DataBits() noexcept : lcsm::DataBits(lcsm::Width::Bit1) {}

lcsm::DataBits::DataBits(lcsm::Width width) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Strength::StrongDrive, lcsm::verilog::Bit::False)
{
}

lcsm::DataBits::DataBits(lcsm::Width width, const lcsm::verilog::Value &value) noexcept : m_width(width), m_bits(value)
{
}

lcsm::DataBits::DataBits(lcsm::Width width, lcsm::verilog::Value &&value) noexcept :
	m_width(width), m_bits(std::move(value))
{
}

lcsm::DataBits::DataBits(lcsm::Width width, lcsm::verilog::Bit bit) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Strength::StrongDrive, bit)
{
}

lcsm::DataBits::DataBits(lcsm::Width width, lcsm::verilog::Strength strength, lcsm::verilog::Bit bit) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Value(strength, bit))
{
}

lcsm::DataBits::DataBits(std::initializer_list< lcsm::verilog::Bit > bits)
{
	const std::size_t width = bits.size();

	/* Check compatible size via switch-case optimizations. */
	switch (width)
	{
	case Bit1:
	case Bit2:
	case Bit3:
	case Bit4:
	case Bit5:
	case Bit6:
	case Bit7:
	case Byte:
	case Word:
	case ShortWord:
	case DoubleWord:
	case QuadWord:
		break;
	default:
		throw std::logic_error("Incompatible size for DataBits");
	}

	/* Fill. */
	std::size_t i = 0;
	for (lcsm::verilog::Bit b : bits)
		m_bits[i++] = { b };

	m_width = static_cast< lcsm::Width >(width);
}

lcsm::DataBits::DataBits(std::initializer_list< lcsm::verilog::Value > values)
{
	const std::size_t width = values.size();

	/* Check compatible size via switch-case optimizations. */
	switch (width)
	{
	case Bit1:
	case Bit2:
	case Bit3:
	case Bit4:
	case Bit5:
	case Bit6:
	case Bit7:
	case Byte:
	case Word:
	case ShortWord:
	case DoubleWord:
	case QuadWord:
		break;
	default:
		throw std::logic_error("Incompatible size for DataBits");
	}

	/* Fill. */
	std::size_t i = 0;
	for (const lcsm::verilog::Value &v : values)
		m_bits[i++] = v;

	m_width = static_cast< lcsm::Width >(width);
}

lcsm::DataBits::DataBits(const DataBits &other) noexcept : m_width(other.m_width), m_bits(other.m_bits) {}

lcsm::DataBits::DataBits(DataBits &&other) noexcept : m_width(std::move(other.m_width)), m_bits(std::move(other.m_bits))
{
}

lcsm::DataBits &lcsm::DataBits::operator=(const lcsm::DataBits &other) noexcept
{
	if (this != std::addressof(other))
		lcsm::DataBits(other).swap(*this);
	return *this;
}

lcsm::DataBits &lcsm::DataBits::operator=(lcsm::DataBits &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::DataBits(std::move(other)).swap(*this);
	return *this;
}

void lcsm::DataBits::swap(lcsm::DataBits &other) noexcept
{
	std::swap(m_width, other.m_width);
	std::swap(m_bits, other.m_bits);
}

lcsm::Width lcsm::DataBits::width() const noexcept
{
	return m_width;
}

bool lcsm::DataBits::checkWidth(const lcsm::DataBits &other) const noexcept
{
	return m_width == other.width();
}

lcsm::verilog::Bit lcsm::DataBits::bit(std::size_t index) const
{
	return value(index).bit();
}

lcsm::verilog::Value &lcsm::DataBits::value(std::size_t index)
{
	return m_bits[index];
}

const lcsm::verilog::Value &lcsm::DataBits::value(std::size_t index) const
{
	return m_bits[index];
}

lcsm::verilog::Value &lcsm::DataBits::operator[](std::size_t index)
{
	return value(index);
}

const lcsm::verilog::Value &lcsm::DataBits::operator[](std::size_t index) const
{
	return value(index);
}

void lcsm::DataBits::setBit(std::size_t index, lcsm::verilog::Bit newBit)
{
	value(index).setBit(newBit);
}

void lcsm::DataBits::setValue(std::size_t index, const lcsm::verilog::Value &newValue)
{
	m_bits[index] = newValue;
}

void lcsm::DataBits::setValue(std::size_t index, lcsm::verilog::Value &&newValue)
{
	m_bits[index] = std::move(newValue);
}
