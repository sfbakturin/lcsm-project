#include <lcsm/Model/Width.h>

#include <stdexcept>
#include <string>

lcsm::Width lcsm::StrongToWidth(unsigned long long width)
{
	switch (width)
	{
	case lcsm::Width::Bit1:
	{
		return lcsm::Width::Bit1;
	}
	case lcsm::Width::Bit2:
	{
		return lcsm::Width::Bit2;
	}
	case lcsm::Width::Bit3:
	{
		return lcsm::Width::Bit3;
	}
	case lcsm::Width::Bit4:
	{
		return lcsm::Width::Bit4;
	}
	case lcsm::Width::Bit5:
	{
		return lcsm::Width::Bit5;
	}
	case lcsm::Width::Bit6:
	{
		return lcsm::Width::Bit6;
	}
	case lcsm::Width::Bit7:
	{
		return lcsm::Width::Bit7;
	}
	case lcsm::Width::Byte:
	{
		return lcsm::Width::Byte;
	}
	case lcsm::Width::Word:
	{
		return lcsm::Width::Word;
	}
	case lcsm::Width::ShortWord:
	{
		return lcsm::Width::ShortWord;
	}
	case lcsm::Width::DoubleWord:
	{
		return lcsm::Width::DoubleWord;
	}
	case lcsm::Width::QuadWord:
	{
		return lcsm::Width::QuadWord;
	}
	default:
	{
		break;
	}
	}
	throw std::logic_error("Provided width is not strongly exactly real width.");
}

std::string lcsm::WidthToString(lcsm::Width width)
{
	return std::string(lcsm::WidthToCStr(width));
}

const char *lcsm::WidthToCStr(lcsm::Width width) noexcept
{
	switch (width)
	{
	case lcsm::Width::Bit1:
	{
		return "1 bit";
	}
	case lcsm::Width::Bit2:
	{
		return "2 bits";
	}
	case lcsm::Width::Bit3:
	{
		return "3 bits";
	}
	case lcsm::Width::Bit4:
	{
		return "4 bits";
	}
	case lcsm::Width::Bit5:
	{
		return "5 bits";
	}
	case lcsm::Width::Bit6:
	{
		return "6 bits";
	}
	case lcsm::Width::Bit7:
	{
		return "7 bits";
	}
	case lcsm::Width::Byte:
	{
		return "byte";
	}
	case lcsm::Width::Word:
	{
		return "word";
	}
	case lcsm::Width::ShortWord:
	{
		return "shord word";
	}
	case lcsm::Width::DoubleWord:
	{
		return "double word";
	}
	case lcsm::Width::QuadWord:
	{
		return "quad word";
	}
	default:
	{
		break;
	}
	}
	return "";
}
