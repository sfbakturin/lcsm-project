#include <lcsm/LCSM.h>

const char *lcsm::CircuitTypePretty(lcsm::ComponentType componentType) noexcept
{
	switch (componentType)
	{
	case lcsm::ComponentType::Wire:
	{
		return "Wire";
	}
	case lcsm::ComponentType::Tunnel:
	{
		return "Tunnel";
	}
	case lcsm::ComponentType::Pin:
	{
		return "Pin";
	}
	case lcsm::ComponentType::Constant:
	{
		return "Constant";
	}
	case lcsm::ComponentType::Power:
	{
		return "Power";
	}
	case lcsm::ComponentType::Ground:
	{
		return "Ground";
	}
	case lcsm::ComponentType::Clock:
	{
		return "Clock";
	}
	case lcsm::ComponentType::Transistor:
	{
		return "Transistor";
	}
	case lcsm::ComponentType::TransmissionGate:
	{
		return "TransmissionGate";
	}
	case lcsm::ComponentType::Button:
	{
		return "Button";
	}
	case lcsm::ComponentType::Digit:
	{
		return "Digit";
	}
	case lcsm::ComponentType::Probe:
	{
		return "Probe";
	}
	case lcsm::ComponentType::Splitter:
	{
		return "Splitter";
	}
	case lcsm::ComponentType::VerilogModule:
	{
		return "VerilogModule";
	}
	default:
		break;
	}
	return nullptr;
}
