#include <lcsm/IR/Bit.h>

lcsm::LogisimBit VerilogToLogisim(lcsm::VerilogBit bit) noexcept
{
	switch (bit)
	{
	case lcsm::VERILOG_TRUE:
		return lcsm::LOGISIM_TRUE;
	case lcsm::VERILOG_FALSE:
		return lcsm::LOGISIM_FALSE;
	case lcsm::VERILOG_UNDEFINED:
	case lcsm::VERILOG_IMPEDANCE:
		return lcsm::LOGISIM_UNDEFINED;
	}
	return lcsm::LOGISIM_ERROR;
}
