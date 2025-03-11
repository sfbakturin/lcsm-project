#include <lcsm/LCSM.h>

const char *lcsm::CircuitTypePretty(lcsm::CircuitType circuitType) noexcept
{
	switch (circuitType)
	{
	case lcsm::CircuitType::Wire:
	{
		return "Wire";
	}
	case lcsm::CircuitType::Tunnel:
	{
		return "Tunnel";
	}
	case lcsm::CircuitType::Pin:
	{
		return "Pin";
	}
	case lcsm::CircuitType::Constant:
	{
		return "Constant";
	}
	case lcsm::CircuitType::Power:
	{
		return "Power";
	}
	case lcsm::CircuitType::Ground:
	{
		return "Ground";
	}
	case lcsm::CircuitType::Clock:
	{
		return "Clock";
	}
	case lcsm::CircuitType::Transistor:
	{
		return "Transistor";
	}
	case lcsm::CircuitType::TransmissionGate:
	{
		return "TransmissionGate";
	}
	case lcsm::CircuitType::Button:
	{
		return "Button";
	}
	case lcsm::CircuitType::Digit:
	{
		return "Digit";
	}
	case lcsm::CircuitType::Probe:
	{
		return "Probe";
	}
	case lcsm::CircuitType::Splitter:
	{
		return "Splitter";
	}
	case lcsm::CircuitType::VerilogModule:
	{
		return "VerilogModule";
	}
	default:
		break;
	}
	return nullptr;
}
