#include <lcsm/Model/Width.h>

#include <stdexcept>

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
