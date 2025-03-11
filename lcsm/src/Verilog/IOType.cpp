#include <lcsm/Verilog/Port.h>

const char *lcsm::verilog::IOTypePretty(lcsm::verilog::IOType ioType) noexcept
{
	switch (ioType)
	{
	case lcsm::verilog::IOType::UnknowPortType:
	{
		return nullptr;
	}
	case lcsm::verilog::IOType::Input:
	{
		return "input";
	}
	case lcsm::verilog::IOType::Inout:
	{
		return "input";
	}
	case lcsm::verilog::IOType::Output:
	{
		return "output";
	}
	case lcsm::verilog::IOType::OutputReg:
	{
		return "outputreg";
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}
