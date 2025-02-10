#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/PinInput.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>

#include <stdexcept>
#include <utility>
#include <vector>

const lcsm::DataBits &lcsm::physical::PinInput::read() const
{
	return m_context->getValue();
}

lcsm::Width lcsm::physical::PinInput::width() const
{
	return read().width();
}

bool lcsm::physical::PinInput::checkWidth(const lcsm::DataBits &value) const
{
	return width() == value.width();
}

lcsm::NodeType lcsm::physical::PinInput::nodeType() const noexcept
{
	return lcsm::NodeType::Dynamic;
}

std::size_t lcsm::physical::PinInput::contextSize() const noexcept
{
	return 1;
}

void lcsm::physical::PinInput::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	m_context = context;
}

void lcsm::physical::PinInput::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::PinInput::addInstant(const lcsm::Instruction &instruction)
{
	/* If caller is external wire and it's WV, then we should invoke this instruction in future. */
	if (instruction.caller() == m_externalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
	{
		m_instants.push_back(instruction);
		return;
	}
	else /* If caller is internal wire and it's WV, then we ignore it without errors. */
		if (instruction.caller() == m_internalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
			return;

	/* Otherwise, no instructions can be provided to input Pin. */
	throw std::logic_error("Attempt to instant instruction for input Pin");
}

void lcsm::physical::PinInput::addInstant(lcsm::Instruction &&instruction)
{
	/* If caller is external wire and it's WV, then we should invoke this instruction in future. */
	if (instruction.caller() == m_externalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
	{
		m_instants.push_back(std::move(instruction));
		return;
	}
	else /* If caller is internal wire and it's WV, then we ignore it without errors. */
		if (instruction.caller() == m_internalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
			return;

	/* Otherwise, no instructions can be provided to input Pin. */
	throw std::logic_error("Attempt to instant instruction for input Pin");
}

std::vector< lcsm::Event > lcsm::physical::PinInput::invokeInstants(const lcsm::Timestamp &now)
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

	/* Traverse value on instants from external connection. */
	for (const lcsm::Instruction &instant : m_instants)
		value |= instant.value();
	m_instants.clear();

	/* Update context value. */
	m_context->updateValues(now, { value });

	/* Resulting events for future mini-steps. */
	std::vector< lcsm::Event > events;

	/* Target from this object. */
	const lcsm::support::PointerView< lcsm::EvaluatorNode > targetFrom = static_cast< lcsm::EvaluatorNode * >(this);
	lcsm::EvaluatorNode *c = m_internalConnect.ptr();

	/* Write value to Wire. */
	lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, c);
	lcsm::Event e = lcsm::CreateInstantEvent(std::move(i), targetFrom, m_internalConnect);
	events.push_back(std::move(e));

	return events;
}

void lcsm::physical::PinInput::connectInternal(const lcsm::support::PointerView< lcsm::EvaluatorNode > &internal)
{
	m_internalConnect = internal;
}

void lcsm::physical::PinInput::connectExternal(const lcsm::support::PointerView< lcsm::EvaluatorNode > &external)
{
	m_externalConnect = external;
}
