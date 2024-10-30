#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Instruction.h>

lcsm::UpdateState::UpdateState(lcsm::CGObject *target) noexcept : m_target(target) {}

void lcsm::UpdateState::invoke() {}

lcsm::InstructionT lcsm::UpdateState::T() const noexcept
{
	return lcsm::InstructionT::INSTR_UPDATE_ELEMENT_STATE;
}

bool lcsm::UpdateState::mayStabilized() const noexcept
{
	return false;
}
