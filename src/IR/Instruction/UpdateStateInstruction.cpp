#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>

sim::UpdateState::UpdateState(sim::CGObject *target) noexcept : m_target(target)
{
}

void sim::UpdateState::invoke() {}

sim::InstructionT sim::UpdateState::T() const noexcept
{
	return sim::InstructionT::INSTR_UPDATE_ELEMENT_STATE;
}

bool sim::UpdateState::mayStabilized() const noexcept
{
	return false;
}
