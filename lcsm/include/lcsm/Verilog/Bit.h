#ifndef LCSM_VERILOG_BIT_H
#define LCSM_VERILOG_BIT_H

#include <ostream>

namespace lcsm
{
	namespace verilog
	{
		enum Bit : signed
		{
			Undefined = -1,
			False = 0,
			True = 1,
		};

		std::ostream &operator<<(std::ostream &os, const Bit &b);
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_BIT_H */
