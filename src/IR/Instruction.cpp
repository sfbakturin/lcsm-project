#include <sim/IR/Instruction.h>

bool sim::Instruction::IsWrite() const noexcept
{
	return T() == sim::InstructionT::INSTR_WRITE_VALUE;
}

bool sim::Instruction::IsRead() const noexcept
{
	return T() == sim::InstructionT::INSTR_READ_VALUE;
}

bool sim::Instruction::IsBroadcast() const noexcept
{
	return T() == sim::InstructionT::INSTR_BROADCAST_VALUE;
}
