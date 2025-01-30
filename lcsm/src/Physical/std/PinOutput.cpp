#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/PinOutput.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>

#include <stdexcept>
#include <utility>
#include <vector>

const lcsm::DataBits &lcsm::physical::PinOutput::read() const
{
	return m_context->getValue();
}

lcsm::Width lcsm::physical::PinOutput::width() const
{
	return read().width();
}

bool lcsm::physical::PinOutput::checkWidth(const lcsm::DataBits &value) const
{
	return width() == value.width();
}

lcsm::NodeType lcsm::physical::PinOutput::nodeType() const noexcept
{
	return lcsm::NodeType::Dynamic;
}

void lcsm::physical::PinOutput::setContext(support::PointerView< Context > &context) noexcept
{
	m_context = context;
}

void lcsm::physical::PinOutput::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::PinOutput::addInstant(const lcsm::Instruction &instruction)
{
	/* If caller is internal wire and it's WV, then we should invoke this instruction in future. */
	if (instruction.caller() == m_internalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
	{
		m_instants.push_back(instruction);
		return;
	}
	else /* If caller is external wire and it's WV, then we ignore it without errors. */
		if (instruction.caller() == m_externalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
			return;

	/* Otherwise, no instructions can be provided to input Pin. */
	throw std::logic_error("Attempt to instant instruction for input Pin");
}

void lcsm::physical::PinOutput::addInstant(lcsm::Instruction &&instruction)
{
	/* If caller is internal wire and it's WV, then we should invoke this instruction in future. */
	if (instruction.caller() == m_internalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
	{
		m_instants.push_back(std::move(instruction));
		return;
	}
	else /* If caller is external wire and it's WV, then we ignore it without errors. */
		if (instruction.caller() == m_externalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
			return;

	/* Otherwise, no instructions can be provided to input Pin. */
	throw std::logic_error("Attempt to instant instruction for input Pin");
}

std::vector< lcsm::Event > lcsm::physical::PinOutput::invokeInstants(const lcsm::Timestamp &now)
{
	/* Invoke all write values into output Pin. */
	lcsm::DataBits result = read();

	for (lcsm::Instruction &instant : m_instants)
	{
		/* All instructions must be WriteValue instance. */
		/* FIXME: Not only WriteValue is optional. */
		// if (!instant.type() == lcsm::)
		// throw std::logic_error("Only WriteValue is available on output Pin to instant");
		result |= instant.caller()->read();
	}

	/* Save last value. */
	m_context->updateValue(now, result);

	/* Clear instants. */
	m_instants.clear();

	// TODO: Make events for external connects, if needed.

	/* Output Pin never produces any events. */
	return {};
}

void lcsm::physical::PinOutput::connectInternal(const lcsm::support::PointerView< lcsm::EvaluatorNode > &internal)
{
	m_internalConnect = internal;
}

void lcsm::physical::PinOutput::connectExternal(const lcsm::support::PointerView< lcsm::EvaluatorNode > &external)
{
	m_externalConnect = external;
}
