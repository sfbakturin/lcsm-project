#ifndef LCSM_MODEL_WIDTH_H
#define LCSM_MODEL_WIDTH_H

namespace lcsm
{
	enum Width : unsigned
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
		MaxWidth = QuadWord
	};
}	 // namespace lcsm

#endif /* LCSM_MODEL_WIDTH_H */
