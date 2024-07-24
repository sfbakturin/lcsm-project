#include <sim/IR/Bit.h>

sim::Bit VerilogToLogisim(sim::VerilogBit bit) noexcept
{
	switch (bit)
	{
	case sim::VERILOG_TRUE:
		return sim::LOGISIM_TRUE;
	case sim::VERILOG_FALSE:
		return sim::LOGISIM_FALSE;
	case sim::VERILOG_UNDEFINED:
	case sim::VERILOG_IMPEDANCE:
		return sim::LOGISIM_UNDEFINED;
	}
}
