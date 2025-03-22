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

lcsm::physical::Pin::Pin(lcsm::object_type_t objectType, bool output, Width width) :
	lcsm::EvaluatorNode(objectType), m_output(output), m_width(width), m_wasPolluteInstant(false)
{
}

lcsm::NodeType lcsm::physical::Pin::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

std::size_t lcsm::physical::Pin::contextSize() const noexcept
{
	return 1;
}

std::size_t lcsm::physical::Pin::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Pin::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	// If context already exists, then reset it.
	if (m_context)
	{
		resetContext();
	}

	// Set and verify context.
	m_context = context;
	verifyContext();
}

void lcsm::physical::Pin::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Pin::verifyContext()
{
	// Check global sizes.
	if (m_context->size() != contextSize() || m_context->privateSize() != privateContextSize())
	{
		resetContext();
		throw std::logic_error("Bad context size!");
	}

	// Check value width, only when there was an update at once.
	if (m_context->neverUpdate())
	{
		return;
	}

	const lcsm::DataBits &value = m_context->getValue();
	if (value.width() != m_width)
	{
		resetContext();
		throw std::logic_error("Bad value width!");
	}
}

void lcsm::physical::Pin::addInstant(const lcsm::Instruction &instruction)
{
	/* If caller is internal or external wire and it's PV, then we flag, that there was pollution. */
	if ((instruction.caller() == m_internalConnect || instruction.caller() == m_externalConnect) &&
		instruction.type() == lcsm::InstructionType::PolluteValue)
	{
		if (m_output == false)
		{
			m_wasPolluteInstant = true;
		}
		return;
	}

	if (!m_output)
	{
		/* If caller is external wire and it's WV, then we should invoke this instruction in future. */
		/* If caller is internal wire and it's WV, then we ignore it without errors. */
		if (instruction.caller() == m_externalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			m_instants.push_back(instruction);
			return;
		}
		else if (instruction.caller() == m_internalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			return;
		}

		/* Otherwise, no instructions can be provided to input Pin. */
		throw std::logic_error("Attempt to instant instruction for input Pin");
	}
	else
	{
		/* If caller is internal wire and it's WV, then we should invoke this instruction in future. */
		/* If caller is external wire and it's WV, then we ignore it without errors. */
		if (instruction.caller() == m_internalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			m_instants.push_back(instruction);
			return;
		}
		else if (instruction.caller() == m_externalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			return;
		}

		/* Otherwise, no instructions can be provided to output Pin. */
		throw std::logic_error("Attempt to instant instruction for output Pin");
	}
}

void lcsm::physical::Pin::addInstant(lcsm::Instruction &&instruction)
{
	/* If caller is internal or external wire and it's PV, then we flag, that there was pollution. */
	if ((instruction.caller() == m_internalConnect || instruction.caller() == m_externalConnect) &&
		instruction.type() == lcsm::InstructionType::PolluteValue)
	{
		if (m_output == false)
		{
			m_wasPolluteInstant = true;
		}
		return;
	}

	if (!m_output)
	{
		/* If caller is external wire and it's WV, then we should invoke this instruction in future. */
		/* If caller is internal wire and it's WV, then we ignore it without errors. */
		if (instruction.caller() == m_externalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			m_instants.push_back(std::move(instruction));
			return;
		}
		else if (instruction.caller() == m_internalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			return;
		}

		/* Otherwise, no instructions can be provided to input Pin. */
		throw std::logic_error("Attempt to instant instruction for input Pin");
	}
	else
	{
		/* If caller is internal wire and it's WV, then we should invoke this instruction in future. */
		/* If caller is external wire and it's WV, then we ignore it without errors. */
		if (instruction.caller() == m_internalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			m_instants.push_back(std::move(instruction));
			return;
		}
		else if (instruction.caller() == m_externalConnect && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			return;
		}

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
		value = instant.value();
	}

	/* Traverse value on instants from external connection. */
	for (const lcsm::Instruction &instant : m_instants)
	{
		value |= instant.value();
	}

	/* Update context value. */
	m_context->updateValues(now, { value });

	/* Clear instants. */
	m_instants.clear();

	/* Resulting events for future mini-steps. */
	std::vector< lcsm::Event > events;

	/* Construct new timestamp with maybe pollution situation. */
	const lcsm::Timestamp diff = lcsm::Timestamp(0, static_cast< lcsm::timescale_t >(m_wasPolluteInstant));
	if (m_output && m_wasPolluteInstant)
	{
		int x = 123;
	}
	m_wasPolluteInstant = false;

	if (m_output)
	{
		if (m_externalConnect)
		{
			/* Write wire's value to target. */
			lcsm::Instruction I = lcsm::CreateWriteValueInstruction(this, m_externalConnect.get(), value);
			lcsm::Event E = { std::move(I), diff };
			events.push_back(std::move(E));
		}
	}
	else
	{
		/* Write value to Wire. */
		lcsm::Instruction I = lcsm::CreateWriteValueInstruction(this, m_internalConnect.get(), value);
		lcsm::Event E = { std::move(I), diff };
		events.push_back(std::move(E));
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
