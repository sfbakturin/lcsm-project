#include <lcsm/Verilog/Bit.h>

#include <ostream>

std::ostream &lcsm::verilog::operator<<(std::ostream &os, const lcsm::verilog::Bit &b)
{
	const char pretty = lcsm::verilog::BitPretty(b);
	if (pretty != 0)
	{
		os << pretty;
	}
	return os;
}

char lcsm::verilog::BitPretty(const lcsm::verilog::Bit &b) noexcept
{
	switch (b)
	{
	case lcsm::verilog::Bit::Undefined:
		return 'x';
	case lcsm::verilog::Bit::False:
		return '0';
	case lcsm::verilog::Bit::True:
		return '1';
	}
	return 0;
}
