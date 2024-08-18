#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>

sim::WriteValueStabilized::WriteValueStabilized(sim::CGObject *targetFrom, sim::CGObject *targetTo) noexcept
	: m_targetFrom(targetFrom), m_targetTo(targetTo)
{
}

void sim::WriteValueStabilized::invoke() {}

sim::InstructionT sim::WriteValueStabilized::T() const noexcept
{
	return sim::InstructionT::INSTR_UPDATE_ELEMENT_STATE;
}

bool sim::WriteValueStabilized::mayStabilized() const noexcept
{
	return false;
}
