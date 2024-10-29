#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Instruction.h>

#include <memory>
#include <utility>

bool lcsm::Instruction::isWrite() const noexcept
{
	return T() == lcsm::InstructionT::INSTR_WRITE_VALUE;
}

bool lcsm::Instruction::isBroadcast() const noexcept
{
	return T() == lcsm::InstructionT::INSTR_BROADCAST_VALUE;
}

bool lcsm::Instruction::isUpdateElementState() const noexcept
{
	return T() == lcsm::InstructionT::INSTR_UPDATE_ELEMENT_STATE;
}

bool lcsm::Instruction::isWriteValueStabilized() const noexcept
{
	return T() == lcsm::InstructionT::INSTR_WRITE_VALUE_STABILIZED;
}

template< typename I, typename... Args >
static std::shared_ptr< lcsm::Instruction > CreateInstruction(Args &&...args)
{
	return std::make_shared< I >(std::forward< Args >(args)...);
}

std::shared_ptr< lcsm::Instruction > lcsm::CreateWriteValue(lcsm::CGObject *targetFrom, lcsm::CGObject *targetTo)
{
	return CreateInstruction< lcsm::WriteValue >(targetFrom, targetTo);
}

std::shared_ptr< lcsm::Instruction > lcsm::CreateBroadcastValue(lcsm::CGObject *targetFrom, lcsm::CGObject *targetTo)
{
	return CreateInstruction< lcsm::BroadcastValue >(targetFrom, targetTo);
}

std::shared_ptr< lcsm::Instruction > lcsm::CreateUpdateState(lcsm::CGObject *target)
{
	return CreateInstruction< lcsm::UpdateState >(target);
}
