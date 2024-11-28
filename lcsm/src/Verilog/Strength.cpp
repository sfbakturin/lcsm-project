#include <lcsm/Verilog/Strength.h>

#include <ostream>

std::ostream &lcsm::verilog::operator<<(std::ostream &os, const lcsm::verilog::Strength &s)
{
	switch (s)
	{
	case HighImpedance:
		return os << "highz";
	case WeakDrive:
		return os << "weak";
	case PullDrive:
		return os << "pull";
	case StrongDrive:
		return os << "strong";
	case SupplyDrive:
		return os << "supply";
	}
}
