#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>

sim::UpdateElementState::UpdateElementState(sim::CGObject *target) noexcept :
	m_target(target)
{
}

void sim::UpdateElementState::invoke() {}

sim::InstructionT sim::UpdateElementState::T() const noexcept
{
	return sim::InstructionT::INSTR_UPDATE_ELEMENT_STATE;
}

bool sim::UpdateElementState::mayStabilized() const noexcept
{
	return false;
}
