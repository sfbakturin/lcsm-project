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
	lcsm::EvaluatorNode(objectType), m_output(output), m_width(width), m_wasPolluted(false)
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

void lcsm::physical::Pin::add(lcsm::Instruction &&instruction)
{
	/* If caller is internal or external wire and it's PV, then we flag, that there was pollution. */
	if ((m_internalConnect == instruction.caller() || m_externalConnect == instruction.caller()) &&
		instruction.type() == lcsm::InstructionType::PolluteValue)
	{
		if (m_output == false)
		{
			m_wasPolluted = true;
		}
		return;
	}

	if (!m_output)
	{
		/* If caller is external wire and it's WV, then we should invoke this instruction in future. */
		/* If caller is internal wire and it's WV, then we ignore it without errors. */
		if (m_externalConnect == instruction.caller() && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			m_instants.push_back(std::move(instruction));
			return;
		}
		else if (m_internalConnect == instruction.caller() && instruction.type() == lcsm::InstructionType::WriteValue)
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
		if (m_internalConnect == instruction.caller() && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			m_instants.push_back(std::move(instruction));
			return;
		}
		else if (m_externalConnect == instruction.caller() && instruction.type() == lcsm::InstructionType::WriteValue)
		{
			return;
		}

		/* Otherwise, no instructions can be provided to output Pin. */
		throw std::logic_error("Attempt to instant instruction for output Pin");
	}
}

std::vector< lcsm::Event > lcsm::physical::Pin::invoke(const lcsm::Timestamp &now)
{
	/* Resulting events for future mini-steps. */
	std::vector< lcsm::Event > events;

	/* If it was polluted, then generate simulator's event. Otherwise, act normally. */
	if (m_wasPolluted)
	{
		/* Create new simulator instruction. */
		events.emplace_back(lcsm::CreatePolluteCircuitSimulatorInstruction(this));

		/* Reset. */
		m_wasPolluted = false;

		/* Return events. */
		return events;
	}

	/* Invoke instants from external connect. */
	lcsm::DataBits value = m_context->getValue();
	const lcsm::Timestamp then = m_context->lastUpdate();
	const bool takeFirst = now > then;
	bool popedInstant = false;

	/* If NOW is later, then THEN, then we should take first value as not-dirty. */
	if (takeFirst && !m_instants.empty())
	{
		lcsm::Instruction &instruction = m_instants.front();
		value = std::move(instruction.value());
		m_instants.pop_front();
		popedInstant = true;
	}

	/* Traverse value on instants from external connection. */
	for (const lcsm::Instruction &instruction : m_instants)
	{
		value |= instruction.value();
	}

	/* Update context value. */
	if (!m_instants.empty() || popedInstant)
	{
		m_context->updateValues(now, { value });
	}

	/* Clear instants. */
	m_instants.clear();

	if (m_output)
	{
		if (m_externalConnect)
		{
			/* Write wire's value to target. */
			events.emplace_back(lcsm::CreateWriteValueInstruction(this, m_externalConnect.get(), value));
		}
	}
	else
	{
		/* Write value to Wire. */
		events.emplace_back(lcsm::CreateWriteValueInstruction(this, m_internalConnect.get(), value));
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
