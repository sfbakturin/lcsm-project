#include <initializer_list>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>

lcsm::DataBits::DataBits() noexcept : lcsm::DataBits(lcsm::Width::Bit1) {}

lcsm::DataBits::DataBits(lcsm::Width width) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Strength::HighImpedance, lcsm::verilog::Bit::Undefined)
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

static std::size_t CheckBitsSize(std::size_t width)
{
	/* Check compatible size via switch-case optimizations. */
	switch (width)
	{
	case lcsm::Width::Bit1:
	case lcsm::Width::Bit2:
	case lcsm::Width::Bit3:
	case lcsm::Width::Bit4:
	case lcsm::Width::Bit5:
	case lcsm::Width::Bit6:
	case lcsm::Width::Bit7:
	case lcsm::Width::Byte:
	case lcsm::Width::Word:
	case lcsm::Width::ShortWord:
	case lcsm::Width::DoubleWord:
	case lcsm::Width::QuadWord:
		return width;
	default:
		throw std::logic_error("Incompatible size for DataBits");
	}
}

lcsm::DataBits::DataBits(std::initializer_list< lcsm::verilog::Bit > bits)
{
	const std::size_t width = CheckBitsSize(bits.size());

	std::size_t i = 0;
	for (lcsm::verilog::Bit b : bits)
		m_bits[i++] = { b };

	m_width = static_cast< lcsm::Width >(width);
}

lcsm::DataBits::DataBits(std::initializer_list< lcsm::verilog::Value > values)
{
	const std::size_t width = CheckBitsSize(values.size());

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
	return lcsm::support::CopyAssign< lcsm::DataBits >(this, other);
}

lcsm::DataBits &lcsm::DataBits::operator=(lcsm::DataBits &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::DataBits >(this, std::move(other));
}

lcsm::DataBits lcsm::DataBits::operator|(const lcsm::DataBits &other)
{
	if (!checkWidth(other))
		throw std::logic_error("Incompatible widths found");

	const unsigned n = static_cast< unsigned >(m_width);
	lcsm::DataBits result = *this;

	for (unsigned i = 0; i < n; i++)
		result[i] |= other[i];

	return result;
}

lcsm::DataBits &lcsm::DataBits::operator|=(const lcsm::DataBits &other)
{
	if (this != std::addressof(other))
		*this = *this | other;
	return *this;
}

bool lcsm::DataBits::operator==(const lcsm::DataBits &other) const noexcept
{
	if (!checkWidth(other))
		return false;

	const unsigned n = static_cast< unsigned >(m_width);

	for (unsigned i = 0; i < n; i++)
		if (value(i) != other[i])
			return false;

	return true;
}

bool lcsm::DataBits::operator!=(const lcsm::DataBits &other) const noexcept
{
	return !(*this == other);
}

void lcsm::DataBits::swap(lcsm::DataBits &other) noexcept
{
	std::swap(m_width, other.m_width);
	std::swap(m_bits, other.m_bits);
}

lcsm::DataBits lcsm::DataBits::fromModel(lcsm::Width width, lcsm::value_t value) noexcept
{
	lcsm::DataBits databits(width);
	unsigned w = static_cast< unsigned >(width);
	std::size_t index = 0;

	while (w != 0)
	{
		const lcsm::verilog::Bit bit = static_cast< lcsm::verilog::Bit >(value & 1);
		databits.setValue(index, { bit });
		index++;
		w--;
		value >>= 1;
	}

	return databits;
}

lcsm::Width lcsm::DataBits::width() const noexcept
{
	return m_width;
}

void lcsm::DataBits::setWidth(lcsm::Width newWidth) noexcept
{
	m_width = newWidth;
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

void lcsm::DataBits::reset() noexcept
{
	const unsigned n = static_cast< unsigned >(width());

	for (unsigned i = 0; i < n; i++)
		m_bits[i] = { lcsm::verilog::Strength::HighImpedance, lcsm::verilog::Bit::Undefined };
}

namespace lcsm
{
	std::ostream &operator<<(std::ostream &os, const lcsm::DataBits &db)
	{
		os << '[';
		const unsigned n = static_cast< unsigned >(db.m_width);
		bool needsComma = false;
		for (unsigned i = 0; i < n; i++)
		{
			if (needsComma)
				os << ',';
			os << db.m_bits[i];
			needsComma = true;
		}
		return os << ']';
	}
}	 // namespace lcsm
