#include <lcsm/Verilog/Port.h>

const char *lcsm::verilog::PortDirectionTypePretty(lcsm::verilog::PortDirectionType portDirectionType) noexcept
{
	switch (portDirectionType)
	{
	case lcsm::verilog::PortDirectionType::UnknowPortDirectionType:
	{
		return nullptr;
	}
	case lcsm::verilog::PortDirectionType::Input:
	{
		return "input";
	}
	case lcsm::verilog::PortDirectionType::Inout:
	{
		return "input";
	}
	case lcsm::verilog::PortDirectionType::Output:
	{
		return "output";
	}
	case lcsm::verilog::PortDirectionType::OutputReg:
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
