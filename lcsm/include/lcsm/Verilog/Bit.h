#ifndef LCSM_VERILOG_BIT_H
#define LCSM_VERILOG_BIT_H

#include <lcsm/lcsmconfig.h>

#include <array>
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

		static constexpr std::array< Bit, 2 > Bits = { Bit::False, Bit::True };

		LCSM_API std::ostream &operator<<(std::ostream &os, const Bit &b);
		LCSM_API const char *BitPretty(const Bit &b) noexcept;
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_BIT_H */
