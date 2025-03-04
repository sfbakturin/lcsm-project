#ifndef LCSM_VERILOG_BIT_H
#define LCSM_VERILOG_BIT_H

#include <cstdint>
#include <ostream>

namespace lcsm
{
	namespace verilog
	{
		enum Bit : std::int8_t
		{
			Undefined = -1,
			False = 0,
			True = 1,
		};

		std::ostream &operator<<(std::ostream &os, const Bit &b);
		char BitPretty(const Bit &b) noexcept;
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_BIT_H */
