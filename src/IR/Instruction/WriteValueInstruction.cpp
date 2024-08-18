#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
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

static bool CondProh(sim::CGObjectT fromT, sim::CGObjectT toT) noexcept
{
	if (fromT == sim::CGObjectT::OBJ_WIRE && toT == sim::CGObjectT::OBJ_PIN_INPUT)
		return true;
	else
		return false;
}

sim::WriteValue::WriteValue(sim::CGObject *targetFrom, sim::CGObject *targetTo) noexcept
	: m_targetFrom(targetFrom), m_targetTo(targetTo)
{
}

void sim::WriteValue::invoke()
{
	const sim::CGObjectT fromT = m_targetFrom->T();
	const sim::CGObjectT toT = m_targetTo->T();

	if (writeable(fromT, toT) && m_targetFrom->checkWidth(m_targetTo->read()))
	{
		// Permitted combination: from PinOutput to Wire, Wire to Wire.
		sim::Value value = m_targetFrom->read();
		m_targetTo->write(std::move(value));
	}
	else if (CondProh(fromT, toT) && m_targetFrom->read() == m_targetTo->read())
	{
		// Conditionally prohibited: when Wire to PinInput, for stabilized
		// situation, writing value must equals to PinInput value, otherwise --
		// error.
		// TODO: Signal somehow stabilization -- not just boolean value.
	}
	else
	{
		// Forbidden combination. For example, attempt to writing PinInput other
		// value.
		throw std::logic_error("Not implemented");
	}
}

sim::InstructionT sim::WriteValue::T() const noexcept
{
	return sim::InstructionT::INSTR_WRITE_VALUE;
}

bool sim::WriteValue::mayStabilized() const noexcept
{
	const sim::CGObjectT fromT = m_targetFrom->T();
	const sim::CGObjectT toT = m_targetTo->T();

	if (writeable(fromT, toT))
		return false;
	else if (CondProh(fromT, toT))
		return true;
	else
		return false;
}
