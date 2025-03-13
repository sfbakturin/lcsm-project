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
#include <string>
#include <utility>

lcsm::DataBits::DataBits() noexcept : lcsm::DataBits(lcsm::Width::Bit1) {}

lcsm::DataBits::DataBits(lcsm::Width width) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Strength::HighImpedance, lcsm::verilog::Bit::Undefined)
{
}

lcsm::DataBits::DataBits(lcsm::Width width, lcsm::verilog::Value value) noexcept : m_width(width), m_bits(value) {}

lcsm::DataBits::DataBits(lcsm::Width width, lcsm::verilog::Bit bit) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Strength::StrongDrive, bit)
{
}

lcsm::DataBits::DataBits(lcsm::Width width, lcsm::verilog::Strength strength, lcsm::verilog::Bit bit) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Value(strength, bit))
{
}

lcsm::DataBits::DataBits(lcsm::verilog::Value value) noexcept : lcsm::DataBits(lcsm::Width::Bit1, value) {}

lcsm::DataBits::DataBits(lcsm::verilog::Bit bit) noexcept : lcsm::DataBits(lcsm::Width::Bit1, bit) {}

lcsm::DataBits::DataBits(lcsm::verilog::Strength strength) noexcept : lcsm::DataBits(lcsm::Width::Bit1, strength) {}

lcsm::DataBits::DataBits(lcsm::verilog::Strength strength, lcsm::verilog::Bit bit) noexcept :
	lcsm::DataBits(lcsm::Width::Bit1, strength, bit)
{
}

lcsm::DataBits::DataBits(lcsm::width_t width) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Strength::HighImpedance, lcsm::verilog::Undefined)
{
}

lcsm::DataBits::DataBits(lcsm::width_t width, lcsm::verilog::Value value) noexcept
{
	m_width = std::min(width, lcsm::MaxWidth);
	for (lcsm::width_t i = 0; i < m_width; i++)
	{
		m_bits[i] = value;
	}
}

lcsm::DataBits::DataBits(lcsm::width_t width, lcsm::verilog::Bit bit) noexcept :
	lcsm::DataBits(width, lcsm::verilog::Strength::StrongDrive, bit)
{
}

lcsm::DataBits::DataBits(lcsm::width_t width, lcsm::verilog::Strength strength, lcsm::verilog::Bit bit) noexcept
{
	m_width = std::min(width, lcsm::MaxWidth);
	for (lcsm::width_t i = 0; i < m_width; i++)
	{
		lcsm::verilog::Value value{ strength, bit };
		m_bits[i] = value;
	}
}

lcsm::DataBits::DataBits(std::initializer_list< lcsm::verilog::Bit > bits) noexcept
{
	m_width = std::min(static_cast< lcsm::width_t >(bits.size()), lcsm::MaxWidth);
	lcsm::width_t i = 0;
	for (lcsm::verilog::Bit bit : bits)
	{
		if (i >= m_width)
		{
			break;
		}
		lcsm::verilog::Value value{ bit };
		m_bits[i] = bit;
		i++;
	}
}

lcsm::DataBits::DataBits(std::initializer_list< std::pair< lcsm::verilog::Strength, lcsm::verilog::Bit > > strengthBits) noexcept
{
	m_width = std::min(static_cast< lcsm::width_t >(strengthBits.size()), lcsm::MaxWidth);
	lcsm::width_t i = 0;
	for (std::pair< lcsm::verilog::Strength, lcsm::verilog::Bit > strengthBit : strengthBits)
	{
		if (i >= m_width)
		{
			break;
		}
		lcsm::verilog::Value value{ strengthBit.first, strengthBit.second };
		m_bits[i] = value;
		i++;
	}
}

lcsm::DataBits::DataBits(std::initializer_list< lcsm::verilog::Value > values) noexcept
{
	m_width = std::min(static_cast< lcsm::width_t >(values.size()), lcsm::MaxWidth);
	lcsm::width_t i = 0;
	for (lcsm::verilog::Value value : values)
	{
		if (i >= m_width)
		{
			break;
		}
		m_bits[i] = value;
		i++;
	}
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
	{
		throw std::logic_error("Incompatible widths found");
	}

	const unsigned n = static_cast< unsigned >(m_width);
	lcsm::DataBits result = *this;

	for (unsigned i = 0; i < n; i++)
	{
		result[i] |= other[i];
	}

	return result;
}

lcsm::DataBits &lcsm::DataBits::operator|=(const lcsm::DataBits &other)
{
	if (this != std::addressof(other))
	{
		*this = *this | other;
	}
	return *this;
}

bool lcsm::DataBits::operator==(const lcsm::DataBits &other) const noexcept
{
	if (!checkWidth(other))
	{
		return false;
	}

	const unsigned n = static_cast< unsigned >(m_width);

	for (unsigned i = 0; i < n; i++)
	{
		if (value(i) != other[i])
		{
			return false;
		}
	}

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

lcsm::DataBits lcsm::DataBits::fromModel(lcsm::Width width, lcsm::value_t v) noexcept
{
	lcsm::DataBits databits(width);
	unsigned w = static_cast< unsigned >(width);
	std::ptrdiff_t index = w - 1;

	while (w != 0)
	{
		const lcsm::verilog::Bit bit{ static_cast< lcsm::verilog::Bit >(v & 1) };
		const lcsm::verilog::Value value{ bit };
		databits.setValue(index, value);
		index--;
		w--;
		v >>= 1;
	}

	return databits;
}

lcsm::DataBits lcsm::DataBits::fromModel(lcsm::value_t value) noexcept
{
	return lcsm::DataBits::fromModel(lcsm::Width::QuadWord, value);
}

lcsm::width_t lcsm::DataBits::width() const noexcept
{
	return m_width;
}

void lcsm::DataBits::setWidth(lcsm::width_t width) noexcept
{
	m_width = std::min(width, lcsm::MaxWidth);
}

void lcsm::DataBits::setWidth(std::size_t width) noexcept
{
	if (width > lcsm::MaxWidth)
	{
		m_width = lcsm::MaxWidth;
	}
	else
	{
		m_width = static_cast< lcsm::width_t >(width);
	}
}

bool lcsm::DataBits::checkWidth(const lcsm::DataBits &other) const noexcept
{
	return m_width == other.width();
}

lcsm::verilog::Bit lcsm::DataBits::bit(std::size_t index) const noexcept
{
	return value(index).bit();
}

lcsm::verilog::Value lcsm::DataBits::value(std::size_t index) const noexcept
{
	static const lcsm::verilog::Value NO_VALUE{ lcsm::verilog::Strength::HighImpedance, lcsm::verilog::Bit::Undefined };
	if (static_cast< lcsm::width_t >(index) >= m_width)
	{
		return NO_VALUE;
	}
	else
	{
		return m_bits[index];
	}
}

lcsm::verilog::Value &lcsm::DataBits::operator[](std::size_t index)
{
	return m_bits[index];
}

const lcsm::verilog::Value &lcsm::DataBits::operator[](std::size_t index) const
{
	return m_bits[index];
}

void lcsm::DataBits::setBit(std::size_t index, lcsm::verilog::Bit bit) noexcept
{
	if (index < m_width)
	{
		m_bits[index].setBit(bit);
	}
}

void lcsm::DataBits::setStrength(std::size_t index, lcsm::verilog::Strength strength) noexcept
{
	if (index < m_width)
	{
		m_bits[index].setStrength(strength);
	}
}

void lcsm::DataBits::setValue(std::size_t index, lcsm::verilog::Value value) noexcept
{
	if (index < m_width)
	{
		m_bits[index] = value;
	}
}

void lcsm::DataBits::set(std::size_t index, verilog::Bit bit, verilog::Strength strength) noexcept
{
	setBit(index, bit);
	setStrength(index, strength);
}

lcsm::DataBits lcsm::DataBits::subdatabits(std::size_t begin) const noexcept
{
	return subdatabits(begin, static_cast< std::size_t >(width()));
}

lcsm::DataBits lcsm::DataBits::subdatabits(std::size_t begin, std::size_t end) const noexcept
{
	const lcsm::width_t size = static_cast< lcsm::width_t >(end - begin);
	lcsm::DataBits sub(size);
	lcsm::width_t j = 0;
	for (lcsm::width_t i = static_cast< lcsm::width_t >(begin); i < static_cast< lcsm::width_t >(end) && i < width(); i++)
	{
		sub.setValue(j, value(i));
		j++;
	}
	return sub;
}

void lcsm::DataBits::reset() noexcept
{
	const unsigned n = static_cast< unsigned >(width());

	for (unsigned i = 0; i < n; i++)
	{
		m_bits[i].reset();
	}
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
			{
				os << ',';
			}
			os << db.m_bits[i];
			needsComma = true;
		}
		return os << ']';
	}
}	 // namespace lcsm

std::string lcsm::DataBits::toString() const
{
	std::string s = "[";
	const unsigned n = static_cast< unsigned >(m_width);
	bool needsComma = false;
	for (unsigned i = 0; i < n; i++)
	{
		if (needsComma)
		{
			s.push_back(',');
		}
		const std::string bitStr = m_bits[i].toString();
		s.append(bitStr);
		needsComma = true;
	}
	s.push_back(']');
	return s;
}
