#ifndef LCSM_VERILOG_STRENGTH_H
#define LCSM_VERILOG_STRENGTH_H

namespace lcsm
{
	namespace verilog
	{
		enum Strength : unsigned
		{
			HighImpedance = 0,
			WeakDrive = 3,
			PullDrive = 5,
			StrongDrive = 6,
			SupplyDrive = 7
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_STRENGTH_H */
