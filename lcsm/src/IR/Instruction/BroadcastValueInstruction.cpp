#include <lcsm/IR/CG.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/IR/Value.h>

#include <stdexcept>
#include <utility>

lcsm::BroadcastValue::BroadcastValue(lcsm::CGObject *targetFrom, lcsm::CGObject *targetTo) noexcept :
	m_targetFrom(targetFrom), m_targetTo(targetTo)
{
}

void lcsm::BroadcastValue::invoke()
{
	if (m_targetFrom->isWire() && m_targetTo->isWire())
	{
		lcsm::Value value = m_targetFrom->read();
		m_targetTo->write(std::move(value));
	}
	else
	{
		throw std::logic_error("Not implemented");
	}
}

lcsm::InstructionT lcsm::BroadcastValue::T() const noexcept
{
	return lcsm::InstructionT::INSTR_BROADCAST_VALUE;
}

bool lcsm::BroadcastValue::mayStabilized() const noexcept
{
	return false;
}
