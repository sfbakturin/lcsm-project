#include <sim/IR/CG.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>

#include <stdexcept>
#include <utility>

sim::BroadcastValue::BroadcastValue(sim::CGObject *targetFrom, sim::CGObject *targetTo) noexcept
	: m_targetFrom(targetFrom), m_targetTo(targetTo)
{
}

void sim::BroadcastValue::invoke()
{
	if (m_targetFrom->isWire() && m_targetTo->isWire())
	{
		sim::Value value = m_targetFrom->read();
		m_targetTo->write(std::move(value));
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

bool sim::BroadcastValue::mayStabilized() const noexcept
{
	return false;
}
