#include <lcsm/LCSM.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_set>

#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Wire::Wire(lcsm::object_type_t objectType) : lcsm::EvaluatorNode(objectType) {}

lcsm::NodeType lcsm::physical::Wire::nodeType() const noexcept
{
	return lcsm::NodeType::Fast;
}

std::size_t lcsm::physical::Wire::contextSize() const noexcept
{
	return 1;
}

std::size_t lcsm::physical::Wire::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Wire::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
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

void lcsm::physical::Wire::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Wire::verifyContext()
{
	// Check global sizes.
	if (m_context->size() != contextSize() || m_context->privateSize() != privateContextSize())
	{
		resetContext();
		throw std::logic_error("Bad context size!");
	}
}

void lcsm::physical::Wire::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

	// Check, if target is this circuit element.
	if (target != this)
	{
		throw std::logic_error("Bad this target element!");
	}

	// Check, if instruction is supported.
	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		m_instants.push_back(std::move(instruction));
		return;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		m_pollutes.push_back(std::move(instruction));
		return;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad instruction!");
}

static inline void
	WireNeighbourInstructions(lcsm::EvaluatorNode *targetFrom, lcsm::EvaluatorNode *targetTo, const lcsm::DataBits &value, std::deque< lcsm::Event > &events)
{
	/* Write wire's value to target. */
	lcsm::Instruction I = lcsm::CreateWriteValueInstruction(targetFrom, targetTo, value);
	events.emplace_back(std::move(I));
}

static inline void WireNeighbourInstructions(
	lcsm::support::PointerView< lcsm::EvaluatorNode > &targetFrom,
	lcsm::support::PointerView< lcsm::EvaluatorNode > &targetTo,
	const lcsm::DataBits &value,
	std::deque< lcsm::Event > &events)
{
	/* Write wire's value to target. */
	WireNeighbourInstructions(targetFrom.get(), targetTo.get(), value, events);
}

void lcsm::physical::Wire::invoke(const lcsm::Timestamp &now, std::deque< lcsm::Event > &events)
{
	lcsm::support::PointerView< lcsm::EvaluatorNode > targetFrom = this;

	/* Check, if there is pollution - then, pollute on all non-callings for pollute. */
	const bool wasPolution = !m_pollutes.empty();
	std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode > > callings;

	/* Act, when there was PolluteValue instruction. */
	if (wasPolution)
	{
		/* Get all polluters. */
		for (lcsm::Instruction &pollute : m_pollutes)
		{
			callings.emplace(pollute.caller());
		}

		/* Go through all objects-neighbour and create a new events to all non callings. */
		for (lcsm::support::PointerView< lcsm::EvaluatorNode > &child : m_children)
		{
			if (callings.find(child) != callings.end())
			{
				continue;
			}

			lcsm::Instruction I = lcsm::CreatePolluteValueInstruction(targetFrom.get(), child.get());
			events.emplace_back(std::move(I));
		}

		/* Clear after pollution. */
		callings.clear();
		m_pollutes.clear();

		/* Set context to polluted. */
		m_context->setPolluted(true);
	}

	/* Don't write value on wires, if there is no need to. */
	const bool wasWrite = !m_instants.empty();

	/* Act, when there was WriteValue instruction. */
	if (wasWrite)
	{
		/* Extract context. */
		lcsm::DataBits value = m_context->getValue();
		const lcsm::Timestamp &then = m_context->lastUpdate();
		const bool takeFirst = now > then;

		/* If NOW is later, then THEN, then we should take first value as not-dirty. */
		if (takeFirst && !m_instants.empty())
		{
			lcsm::Instruction &instruction = m_instants.front();
			value = std::move(instruction.value());
			callings.emplace(instruction.caller());
			m_instants.pop_front();
		}

		/* Invoke all instructions. */
		for (lcsm::Instruction &instruction : m_instants)
		{
			value |= instruction.value();
			callings.emplace(instruction.caller());
		}

		/* Go through all objects-neighbour and create a new events to all non callings. */
		for (lcsm::support::PointerView< lcsm::EvaluatorNode > &child : m_children)
		{
			if (callings.count(child))
			{
				continue;
			}
			WireNeighbourInstructions(targetFrom, child, value, events);
		}

		/* Go through all objects-neighbour and create a new events to all callings, where instant::value() != value. */
		for (lcsm::Instruction &instruction : m_instants)
		{
			if (value != instruction.value())
			{
				WireNeighbourInstructions(this, instruction.caller(), value, events);
			}
		}

		/* Save last value. */
		m_context->updateValues(now, { value });
		m_context->setPolluted(false);

		/* Clear instants. */
		m_instants.clear();
	}
}

void lcsm::physical::Wire::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &child)
{
	m_children.push_back(child);
}
