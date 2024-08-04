#include <sim/IR/CG.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>

#include <stdexcept>
#include <utility>

static bool writeable(sim::CGObjectT fromT, sim::CGObjectT toT) noexcept
{
	if (fromT == sim::CGObjectT::OBJ_PIN_INPUT && toT == sim::CGObjectT::OBJ_WIRE)
		return true;
	else if (fromT == sim::CGObjectT::OBJ_WIRE && toT == sim::CGObjectT::OBJ_PIN_OUTPUT)
		return true;
	else
		return false;
}

sim::WriteValue::WriteValue(sim::CGObject *targetFrom, sim::CGObject *targetTo) noexcept :
	m_targetFrom(targetFrom), m_targetTo(targetTo)
{
}

void sim::WriteValue::Invoke()
{
	if (writeable(m_targetFrom->T(), m_targetTo->T()) && m_targetFrom->CheckWidth(m_targetTo->Read()))
	{
		sim::Value value = m_targetFrom->Read();
		m_targetTo->Write(std::move(value));
	}
	else
	{
		throw std::logic_error("Not implemented");
	}
}

sim::InstructionT sim::WriteValue::T() const noexcept
{
	return sim::InstructionT::INSTR_WRITE_VALUE;
}
