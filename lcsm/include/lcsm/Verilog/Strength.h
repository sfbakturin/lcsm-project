#ifndef LCSM_VERILOG_STRENGTH_H
#define LCSM_VERILOG_STRENGTH_H

#include <cstdint>
#include <ostream>

namespace lcsm
{
	namespace verilog
	{
		enum Strength : std::uint8_t
		{
			HighImpedance = 0,
			WeakDrive = 3,
			PullDrive = 5,
			StrongDrive = 6,
			SupplyDrive = 7
		};

		std::ostream &operator<<(std::ostream &os, const Strength &s);
		const char *StrengthPretty(const Strength &s) noexcept;
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_STRENGTH_H */
