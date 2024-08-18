#include <sim/IR/Instruction.h>

bool sim::Instruction::isWrite() const noexcept
{
	return T() == sim::InstructionT::INSTR_WRITE_VALUE;
}

bool sim::Instruction::isBroadcast() const noexcept
{
	return T() == sim::InstructionT::INSTR_BROADCAST_VALUE;
}

bool sim::Instruction::isUpdateElementState() const noexcept
{
	return T() == sim::InstructionT::INSTR_UPDATE_ELEMENT_STATE;
}

bool sim::Instruction::isWriteValueStabilized() const noexcept
{
	return T() == sim::InstructionT::INSTR_WRITE_VALUE_STABILIZED;
}
