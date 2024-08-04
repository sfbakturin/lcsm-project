#include <sim/IR/CG.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>

#include <stdexcept>
#include <utility>

sim::BroadcastValue::BroadcastValue(sim::CGObject *targetFrom, sim::CGObject *targetTo) noexcept :
	m_targetFrom(targetFrom), m_targetTo(targetTo)
{
}

void sim::BroadcastValue::Invoke()
{
	if (m_targetFrom->IsWire() && m_targetTo->IsWire())
	{
		sim::Value value = m_targetFrom->Read();
		m_targetTo->Write(std::move(value));
	}
	else
	{
		throw std::logic_error("Not implemented");
	}
}

sim::InstructionT sim::BroadcastValue::T() const noexcept
{
	return sim::InstructionT::INSTR_BROADCAST_VALUE;
}
