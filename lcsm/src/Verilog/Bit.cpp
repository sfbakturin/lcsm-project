#include <lcsm/Verilog/Bit.h>

#include <ostream>

std::ostream &lcsm::verilog::operator<<(std::ostream &os, const lcsm::verilog::Bit &b)
{
	switch (b)
	{
	case Undefined:
		return os << 'X';
	case False:
		return os << '0';
	case True:
		return os << '1';
	}
}
