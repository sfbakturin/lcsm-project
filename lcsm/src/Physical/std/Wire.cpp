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

void lcsm::physical::Wire::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	m_context = context;
}

void lcsm::physical::Wire::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Wire::addInstant(const lcsm::Instruction &instruction)
{
	/* Only BroadcastValue and WriteValue instructions are available. */
	if (instruction.type() == lcsm::InstructionType::WriteValue)
		m_instants.push_back(instruction);
	else
		throw std::logic_error("Attempt to instant non BroadcastValue for wire");
}

void lcsm::physical::Wire::addInstant(lcsm::Instruction &&instruction)
{
	/* Only BroadcastValue and WriteValue instructions are available. */
	if (instruction.type() == lcsm::InstructionType::WriteValue)
		m_instants.push_back(std::move(instruction));
	else
		throw std::logic_error("Attempt to instant non BroadcastValue for wire");
}

static inline void
	WireNeighbourInstructions(lcsm::EvaluatorNode *targetFrom, lcsm::EvaluatorNode *targetTo, const lcsm::DataBits &value, std::vector< lcsm::Event > &events)
{
	/* Write wire's value to target. */
	lcsm::Instruction i = lcsm::CreateWriteValueInstruction(targetFrom, targetTo, value);
	events.emplace_back(std::move(i));
}

static inline void WireNeighbourInstructions(
	lcsm::support::PointerView< lcsm::EvaluatorNode > &targetFrom,
	lcsm::support::PointerView< lcsm::EvaluatorNode > &targetTo,
	const lcsm::DataBits &value,
	std::vector< lcsm::Event > &events)
{
	/* Write wire's value to target. */
	WireNeighbourInstructions(targetFrom.get(), targetTo.get(), value, events);
}

std::vector< lcsm::Event > lcsm::physical::Wire::invokeInstants(const lcsm::Timestamp &now)
{
	std::vector< lcsm::Event > events;
	lcsm::support::PointerView< lcsm::EvaluatorNode > targetFrom = this;

	/* Extract context. */
	lcsm::DataBits value = m_context->getValue();
	const lcsm::Timestamp &then = m_context->lastUpdate();
	const bool takeFirst = now > then;

	/* If NOW is later, then THEN, then we should take first value as not-dirty. */
	if (takeFirst && !m_instants.empty())
	{
		const lcsm::Instruction instant = m_instants.front();
		m_instants.pop_front();
		value = instant.value();
	}

	/* Invoke all instructions. */
	std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode > > callings;
	for (lcsm::Instruction &instant : m_instants)
	{
		value |= instant.value();
		instant.caller();
		callings.emplace(instant.caller());
	}

	/* Go through all objects-neighbour and create a new events to all non callings. */
	for (lcsm::support::PointerView< lcsm::EvaluatorNode > &child : m_children)
	{
		if (callings.count(child))
			continue;
		WireNeighbourInstructions(targetFrom, child, value, events);
	}

	/* Go through all objects-neighbour and create a new events to all callings, where instant::value() != value. */
	for (lcsm::Instruction &instant : m_instants)
	{
		if (value != instant.value())
			WireNeighbourInstructions(this, instant.caller(), value, events);
	}

	/* Save last value. */
	m_context->updateValues(now, { value });

	/* Clear instants. */
	m_instants.clear();

	return events;
}

void lcsm::physical::Wire::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &child)
{
	m_children.push_back(child);
}
