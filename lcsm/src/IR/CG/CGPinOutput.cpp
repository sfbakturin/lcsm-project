#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Event.h>
#include <lcsm/IR/Instruction.h>

#include <stdexcept>
#include <utility>

void lcsm::CGPinOutput::reset() noexcept
{
	m_value.reset();
}

lcsm::DataBits &lcsm::CGPinOutput::read()
{
	// TODO: Maybe, it should be as real inout element.
	return m_value;
}

const lcsm::DataBits &lcsm::CGPinOutput::read() const
{
	// TODO: Maybe, it should be as real inout element.
	return m_value;
}

void lcsm::CGPinOutput::write(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGPinOutput::write(lcsm::DataBits &&value)
{
	m_value = std::move(value);
}

lcsm::CGObjectType lcsm::CGPinOutput::objectType() const noexcept
{
	return lcsm::CGObjectType::PinOutput;
}

const lcsm::DataBits &lcsm::CGPinOutput::externalRead() const noexcept
{
	return m_value;
}

lcsm::DataBits &lcsm::CGPinOutput::externalRead() noexcept
{
	return m_value;
}

lcsm::CGPinOutput *lcsm::CGPinOutput::asPinOutput() noexcept
{
	return this;
}

const lcsm::CGPinOutput *lcsm::CGPinOutput::asPinOutput() const noexcept
{
	return this;
}

void lcsm::CGPinOutput::pushBackInstruction(const lcsm::Instruction &instruction)
{
	/* Only WriteValue instruction is available. */
	if (!instruction.isWrite())
		throw std::logic_error("Attempt to instant non WriteValue for output Pin");
	lcsm::CGObject::pushBackInstruction(instruction);
}

void lcsm::CGPinOutput::pushBackInstruction(lcsm::Instruction &&instruction)
{
	/* Only WriteValue instruction is available. */
	if (!instruction.isWrite())
		throw std::logic_error("Attempt to instant non WriteValue for output Pin");
	lcsm::CGObject::pushBackInstruction(std::move(instruction));
}

std::vector< lcsm::Event > lcsm::CGPinOutput::invokeInstructions()
{
	/* Invoke all write values into output Pin. */
	lcsm::DataBits result = m_value;

	for (lcsm::Instruction &instant : m_instants)
	{
		/* All instructions must be WriteValue instance. */
		if (!instant.isWrite())
			throw std::logic_error("Only WriteValue is available on output Pin to instant");
		result |= instant.caller()->read();
	}

	/* Save last value. */
	m_value = result;

	/* Clear instants. */
	m_instants.clear();

	/* Output Pin never produces any events. */
	return {};
}
