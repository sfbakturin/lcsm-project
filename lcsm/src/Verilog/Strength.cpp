#include <lcsm/Verilog/Strength.h>

#include <ostream>

std::ostream &lcsm::verilog::operator<<(std::ostream &os, const lcsm::verilog::Strength &s)
{
	const char *pretty = lcsm::verilog::StrengthPretty(s);
	if (pretty != nullptr)
	{
		os << pretty;
	}
	return os;
}

const char *lcsm::verilog::StrengthPretty(const lcsm::verilog::Strength &s) noexcept
{
	switch (s)
	{
	case lcsm::verilog::Strength::HighImpedance:
		return "highz";
	case lcsm::verilog::Strength::WeakDrive:
		return "weak";
	case lcsm::verilog::Strength::PullDrive:
		return "pull";
	case lcsm::verilog::Strength::StrongDrive:
		return "strong";
	case lcsm::verilog::Strength::SupplyDrive:
		return "supply";
	}
	return nullptr;
}
