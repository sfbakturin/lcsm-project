#ifndef LCSM_MODEL_WIDTH_H
#define LCSM_MODEL_WIDTH_H

#include <lcsm/lcsmconfig.h>

#include <array>
#include <cstdint>
#include <string>

namespace lcsm
{
	using width_t = std::uint8_t;
	using width_bitmask_t = std::uint64_t;

	enum Width : width_t
	{
		Bit1 = 1,
		Bit2 = 2,
		Bit3 = 3,
		Bit4 = 4,
		Bit5 = 5,
		Bit6 = 6,
		Bit7 = 7,
		Byte = 8,
		Word = 16,
		ShortWord = 24,
		DoubleWord = 32,
		QuadWord = 64,
		LastWidth = QuadWord
	};

	static constexpr lcsm::width_t MaxWidth = Width::LastWidth;

	// clang-format off
	constexpr std::array< Width, 11 > Widths =
	{
		Width::Bit1,      Width::Bit2, Width::Bit3,
		Width::Bit4,      Width::Bit5, Width::Bit6,
		Width::Bit7,      Width::Byte,
		Width::ShortWord, Width::DoubleWord,
		Width::QuadWord
	};
	// clang-format on

	LCSM_API Width StrongToWidth(unsigned long long width);
	LCSM_API std::string WidthToString(Width width);
	LCSM_API const char *WidthToCStr(Width width) noexcept;
}	 // namespace lcsm

#endif /* LCSM_MODEL_WIDTH_H */
