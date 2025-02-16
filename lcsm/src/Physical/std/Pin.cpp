#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Pin.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>

#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Pin::Pin(lcsm::object_type_t objectType, bool output) :
	lcsm::EvaluatorNode(objectType), m_output(output)
{
}

const lcsm::DataBits &lcsm::physical::Pin::read() const
{
	return m_context->getValue();
}

lcsm::Width lcsm::physical::Pin::width() const
{
	return read().width();
}

bool lcsm::physical::Pin::checkWidth(const lcsm::DataBits &value) const
{
	return width() == value.width();
}

lcsm::NodeType lcsm::physical::Pin::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

std::size_t lcsm::physical::Pin::contextSize() const noexcept
{
	return 1;
}

void lcsm::physical::Pin::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	m_context = context;
}

void lcsm::physical::Pin::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Pin::addInstant(const lcsm::Instruction &instruction)
{
	if (!m_output)
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
	else
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

		/* Otherwise, no instructions can be provided to output Pin. */
		throw std::logic_error("Attempt to instant instruction for output Pin");
	}
}

void lcsm::physical::Pin::addInstant(lcsm::Instruction &&instruction)
{
	if (!m_output)
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
	else
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

		/* Otherwise, no instructions can be provided to output Pin. */
		throw std::logic_error("Attempt to instant instruction for output Pin");
	}
}

std::vector< lcsm::Event > lcsm::physical::Pin::invokeInstants(const lcsm::Timestamp &now)
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
		value |= instant.caller()->read();

	/* Update context value. */
	m_context->updateValues(now, { value });

	/* Clear instants. */
	m_instants.clear();

	/* Resulting events for future mini-steps. */
	std::vector< lcsm::Event > events;

	if (m_output)
	{
		if (m_externalConnect)
		{
			/* Write wire's value to target. */
			lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, m_externalConnect.ptr());
			events.emplace_back(std::move(i));
		}
	}
	else
	{
		/* Write value to Wire. */
		lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, m_internalConnect.ptr());
		events.push_back(std::move(i));
	}

	return events;
}

void lcsm::physical::Pin::connectInternal(const lcsm::support::PointerView< lcsm::EvaluatorNode > &internal)
{
	m_internalConnect = internal;
}

void lcsm::physical::Pin::connectExternal(const lcsm::support::PointerView< lcsm::EvaluatorNode > &external)
{
	m_externalConnect = external;
}
