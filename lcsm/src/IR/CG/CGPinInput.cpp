#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Event.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>

#include <stdexcept>
#include <utility>
#include <vector>

lcsm::CGPinInput::CGPinInput()
{
	reset();
}

void lcsm::CGPinInput::reset() noexcept
{
	m_value = lcsm::DataBits(lcsm::model::Width::Bit1, lcsm::verilog::Strength::StrongDrive, lcsm::verilog::Bit::False);
}

lcsm::DataBits &lcsm::CGPinInput::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGPinInput::read() const
{
	return m_value;
}

void lcsm::CGPinInput::write(const lcsm::DataBits &)
{
	// Scheduler must prevent attempts to write to the input Pin.
	throw std::logic_error("Attempt to write to the input Pin element.");
}

void lcsm::CGPinInput::write(lcsm::DataBits &&)
{
	// Scheduler must prevent attempts to write to the input Pin.
	throw std::logic_error("Attempt to write to the input Pin element.");
}

lcsm::CGObjectType lcsm::CGPinInput::objectType() const noexcept
{
	return lcsm::CGObjectType::PinInput;
}

void lcsm::CGPinInput::externalWrite(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGPinInput::externalWrite(lcsm::DataBits &&value) noexcept
{
	m_value = std::move(value);
}

lcsm::CGPinInput *lcsm::CGPinInput::asPinInput() noexcept
{
	return this;
}

const lcsm::CGPinInput *lcsm::CGPinInput::asPinInput() const noexcept
{
	return this;
}

void lcsm::CGPinInput::pushBackInstruction(const lcsm::Instruction &)
{
	/* No instructions can be provided to input Pin. */
	throw std::logic_error("Attempt to instant instruction for input Pin");
}

void lcsm::CGPinInput::pushBackInstruction(lcsm::Instruction &&)
{
	/* No instructions can be provided to input Pin. */
	throw std::logic_error("Attempt to instant instruction for input Pin");
}

std::vector< lcsm::Event > lcsm::CGPinInput::invokeInstructions()
{
	/* No instructions can be provided to input Pin. */
	if (!m_instants.empty())
		throw std::logic_error("Attempt to instant instruction for input Pin");

	/* Resulting events for future mini-steps. */
	std::vector< lcsm::Event > events;

	/* Target from this object. */
	const lcsm::support::PointerView< lcsm::CGNode > targetFrom = static_cast< lcsm::CGObject * >(this);

	for (lcsm::support::PointerView< lcsm::CGNode > &child : m_children)
	{
		lcsm::CGObject *c = static_cast< lcsm::CGObject * >(child.ptr());

		/* Only wires are available as pin's childrens. */
		if (!c->isWire())
			throw std::logic_error("Pin connected to something else");

		/* Write value to Wire. */
		lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, c);
		lcsm::Event e = lcsm::CreateInstantEvent(std::move(i), targetFrom, child);
		events.push_back(std::move(e));
	}

	return events;
}
