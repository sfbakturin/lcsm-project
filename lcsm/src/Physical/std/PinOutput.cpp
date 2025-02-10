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

std::size_t lcsm::physical::PinOutput::contextSize() const noexcept
{
	return 1;
}

void lcsm::physical::PinOutput::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
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
	/* Invoke instants from external connect. */
	lcsm::DataBits value = m_context->getValue();
	const lcsm::Timestamp then = m_context->lastUpdate();
	const bool takeFirst = now > then;

	/* If NOW is later, then THEN, then we should take first value as not-dirty. */
	if (takeFirst && !m_instants.empty())
	{
		const lcsm::Instruction instant = m_instants.front();
		m_instants.pop_front();
		value = instant.caller()->read();
	}

	/* Invoke all instructions. */
	for (lcsm::Instruction &instant : m_instants)
		value |= instant.caller()->read();

	/* Save last value. */
	m_context->updateValues(now, { value });

	/* Clear instants. */
	m_instants.clear();

	/* If external connect exists, then, we should create events for writing to output. */
	std::vector< lcsm::Event > events;
	if (m_externalConnect)
	{
		/* Extract target and caller. */
		lcsm::support::PointerView< lcsm::EvaluatorNode > targetFrom = static_cast< lcsm::EvaluatorNode * >(this);
		lcsm::EvaluatorNode *target = static_cast< lcsm::EvaluatorNode * >(m_externalConnect.ptr());
		lcsm::EvaluatorNode *caller = static_cast< lcsm::EvaluatorNode * >(targetFrom.ptr());
		/* Write wire's value to target. */
		lcsm::Instruction i = lcsm::CreateWriteValueInstruction(caller, target);
		lcsm::Event e = lcsm::CreateInstantEvent(std::move(i), targetFrom, m_externalConnect);
		events.push_back(std::move(e));
	}

	return events;
}

void lcsm::physical::PinOutput::connectInternal(const lcsm::support::PointerView< lcsm::EvaluatorNode > &internal)
{
	m_internalConnect = internal;
}

void lcsm::physical::PinOutput::connectExternal(const lcsm::support::PointerView< lcsm::EvaluatorNode > &external)
{
	m_externalConnect = external;
}
