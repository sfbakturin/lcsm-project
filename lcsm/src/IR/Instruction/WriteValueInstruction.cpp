#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Instruction.h>

#include <stdexcept>
#include <utility>

static bool writeable(lcsm::CGObjectT fromT, lcsm::CGObjectT toT) noexcept
{
	if (fromT == lcsm::CGObjectT::OBJ_PIN_INPUT && toT == lcsm::CGObjectT::OBJ_WIRE)
		return true;
	else if (fromT == lcsm::CGObjectT::OBJ_WIRE && toT == lcsm::CGObjectT::OBJ_PIN_OUTPUT)
		return true;
	else
		return false;
}

static bool CondProh(lcsm::CGObjectT fromT, lcsm::CGObjectT toT) noexcept
{
	if (fromT == lcsm::CGObjectT::OBJ_WIRE && toT == lcsm::CGObjectT::OBJ_PIN_INPUT)
		return true;
	else
		return false;
}

lcsm::WriteValue::WriteValue(lcsm::CGObject *targetFrom, lcsm::CGObject *targetTo) noexcept :
	m_targetFrom(targetFrom), m_targetTo(targetTo)
{
}

void lcsm::WriteValue::invoke()
{
	const lcsm::CGObjectT fromT = m_targetFrom->T();
	const lcsm::CGObjectT toT = m_targetTo->T();

	if (writeable(fromT, toT) && m_targetFrom->checkWidth(m_targetTo->read()))
	{
		// Permitted combination: from PinOutput to Wire, Wire to Wire.
		lcsm::DataBits value = m_targetFrom->read();
		m_targetTo->write(std::move(value));
	}
	else if (CondProh(fromT, toT) /* && m_targetFrom->read() == m_targetTo->read() -- it should be rewritten */)
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

lcsm::InstructionT lcsm::WriteValue::T() const noexcept
{
	return lcsm::InstructionT::INSTR_WRITE_VALUE;
}

bool lcsm::WriteValue::mayStabilized() const noexcept
{
	const lcsm::CGObjectT fromT = m_targetFrom->T();
	const lcsm::CGObjectT toT = m_targetTo->T();

	if (writeable(fromT, toT))
		return false;
	else if (CondProh(fromT, toT))
		return true;
	else
		return false;
}
