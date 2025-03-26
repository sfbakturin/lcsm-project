#ifndef LCSM_VERILOG_STRENGTH_H
#define LCSM_VERILOG_STRENGTH_H

#include <lcsm/lcsmconfig.h>

#include <array>
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

		// clang-format off
		static constexpr std::array< Strength, 5 > Strengths = {
			Strength::HighImpedance, Strength::WeakDrive,
			Strength::PullDrive,     Strength::StrongDrive,
			Strength::SupplyDrive
		};
		// clang-format on

		LCSM_API std::ostream &operator<<(std::ostream &os, const Strength &s);
		LCSM_API const char *StrengthPretty(const Strength &s) noexcept;
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_STRENGTH_H */
