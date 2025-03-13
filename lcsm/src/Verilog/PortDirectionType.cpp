#include <lcsm/Verilog/Port.h>

const char* lcsm::verilog::PortDirectionTypePretty(lcsm::verilog::PortDirectionType portDirectionType) noexcept
{
	switch (portDirectionType)
	{
	case lcsm::verilog::PortDirectionType::UnknowPortDirectionType:
	{
		break;
	}
	case lcsm::verilog::PortDirectionType::InputPortDirection:
	{
		return "input";
	}
	case lcsm::verilog::PortDirectionType::InoutPortDirection:
	{
		return "input";
	}
	case lcsm::verilog::PortDirectionType::OutputPortDirection:
	{
		return "output";
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}

const char* lcsm::verilog::IntegerVectorTypePretty(lcsm::verilog::IntegerVectorType integerVectorType) noexcept
{
	switch (integerVectorType)
	{
	case lcsm::verilog::UnknownIntegerVectorType:
	{
		break;
	}
	case lcsm::verilog::BitIntegerVector:
	{
		return "bit";
	}
	case lcsm::verilog::LogicIntegerVector:
	{
		return "logic";
	}
	case lcsm::verilog::RegIntegerVector:
	{
		return "reg";
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}

const char* lcsm::verilog::NetTypePretty(lcsm::verilog::NetType netType) noexcept
{
	switch (netType)
	{
	case lcsm::verilog::UnknownNetType:
	{
		break;
	}
	case lcsm::verilog::Supply0Net:
	{
		return "supply0";
	}
	case lcsm::verilog::Supply1Net:
	{
		return "supply1";
	}
	case lcsm::verilog::TriNet:
	{
		return "tri";
	}
	case lcsm::verilog::TriandNet:
	{
		return "triand";
	}
	case lcsm::verilog::TriorNet:
	{
		return "trior";
	}
	case lcsm::verilog::Tri0Net:
	{
		return "tri0";
	}
	case lcsm::verilog::Tri1Net:
	{
		return "tri1";
	}
	case lcsm::verilog::UwireNet:
	{
		return "uwire";
	}
	case lcsm::verilog::WireNet:
	{
		return "wire";
	}
	case lcsm::verilog::WandNet:
	{
		return "wand";
	}
	case lcsm::verilog::WorNet:
	{
		return "wor";
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}

const char* lcsm::verilog::OutputVariableTypePretty(lcsm::verilog::OutputVariableType outputVariableType) noexcept
{
	switch (outputVariableType)
	{
	case lcsm::verilog::UnknownOutputVariableType:
	{
		break;
	}
	case lcsm::verilog::IntegerOutputVariable:
	{
		return "integer";
	}
	case lcsm::verilog::TimeOutputVariable:
	{
		return "time";
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}
