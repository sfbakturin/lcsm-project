#ifndef LCSM_VERILOG_CAPACITIVE_H
#define LCSM_VERILOG_CAPACITIVE_H

#include <cstdint>

namespace lcsm
{
	namespace verilog
	{
		enum Capacitive : std::uint8_t
		{
			SmallCapacitive = 1,
			MediumCapacitive = 2,
			LargeCapacitive = 4
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif
