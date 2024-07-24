#ifndef SIM_IR_BIT_H
#define SIM_IR_BIT_H

namespace sim
{
	enum VerilogBit : unsigned
	{
		VERILOG_TRUE,
		VERILOG_FALSE,
		VERILOG_UNDEFINED,
		VERILOG_IMPEDANCE
	};

	enum Bit : unsigned
	{
		LOGISIM_TRUE,
		LOGISIM_FALSE,
		LOGISIM_UNDEFINED,
		LOGISIM_ERROR
	};

	Bit VerilogToLogisim(VerilogBit bit) noexcept;
}	 // namespace sim

#endif /* SIM_IR_BIT_H */
