#include <lcsm/Verilog/Strength.h>

#include <ostream>

std::ostream &lcsm::verilog::operator<<(std::ostream &os, const lcsm::verilog::Strength &s)
{
	switch (s)
	{
	case lcsm::verilog::Strength::HighImpedance:
		return os << "highz";
	case lcsm::verilog::Strength::WeakDrive:
		return os << "weak";
	case lcsm::verilog::Strength::PullDrive:
		return os << "pull";
	case lcsm::verilog::Strength::StrongDrive:
		return os << "strong";
	case lcsm::verilog::Strength::SupplyDrive:
		return os << "supply";
	}
	return os;
}
