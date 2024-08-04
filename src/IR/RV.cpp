#include <sim/IR/CG.h>
#include <sim/IR/Instruction.h>

#include <stdexcept>
#include <utility>

sim::ReadValue::ReadValue(sim::CGObject *targetFrom, sim::CGObject *targetTo) noexcept :
	m_targetFrom(targetFrom), m_targetTo(targetTo)
{
}

void sim::ReadValue::Invoke()
{
	if (m_targetFrom->IsPinOutput() && m_targetTo->IsWire())
	{
		sim::Value value = m_targetFrom->Read();
		m_targetTo->Write(std::move(value));
	}
	else
	{
		throw std::logic_error("RV instruction is available only for [Target:From=Pin:Output, Target:To=Wire].");
	}
}

sim::InstructionT sim::ReadValue::T() const noexcept
{
	return sim::InstructionT::INSTR_READ_VALUE;
}
