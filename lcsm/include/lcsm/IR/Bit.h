#ifndef LCSM_IR_BIT_H
#define LCSM_IR_BIT_H

namespace lcsm
{
	enum VerilogBit : unsigned
	{
		VERILOG_TRUE,
		VERILOG_FALSE,
		VERILOG_UNDEFINED,
		VERILOG_IMPEDANCE
	};

	enum LogisimBit : unsigned
	{
		LOGISIM_TRUE,
		LOGISIM_FALSE,
		LOGISIM_UNDEFINED,
		LOGISIM_ERROR
	};

	LogisimBit verilogToLogisim(VerilogBit bit) noexcept;
}	 // namespace lcsm

#endif /* LCSM_IR_BIT_H */
