#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>

#include <memory>
#include <utility>

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

template< typename I, typename... Args >
static std::shared_ptr< sim::Instruction > CreateInstruction(Args &&...args)
{
	return std::make_shared< I >(std::forward< Args >(args)...);
}

std::shared_ptr< sim::Instruction >
	sim::CreateWriteValue(sim::CGObject *targetFrom, sim::CGObject *targetTo)
{
	return CreateInstruction< sim::WriteValue >(targetFrom, targetTo);
}

std::shared_ptr< sim::Instruction >
	sim::CreateBroadcastValue(sim::CGObject *targetFrom, sim::CGObject *targetTo)
{
	return CreateInstruction< sim::BroadcastValue >(targetFrom, targetTo);
}

std::shared_ptr< sim::Instruction > sim::CreateUpdateState(sim::CGObject *target)
{
	return CreateInstruction< sim::UpdateState >(target);
}

std::shared_ptr< sim::Instruction >
	sim::CreateWriteValueStabilized(sim::CGObject *targetFrom, sim::CGObject *targetTo)
{
	return CreateInstruction< sim::WriteValueStabilized >(targetFrom, targetTo);
}
