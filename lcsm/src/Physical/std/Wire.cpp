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

#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::NodeType lcsm::physical::Wire::nodeType() const noexcept
{
	return lcsm::NodeType::Fast;
}

std::size_t lcsm::physical::Wire::contextSize() const noexcept
{
	return 1;
}

const lcsm::DataBits &lcsm::physical::Wire::read() const
{
	return m_context->getValue();
}

lcsm::Width lcsm::physical::Wire::width() const
{
	return read().width();
}

bool lcsm::physical::Wire::checkWidth(const DataBits &value) const
{
	return width() == value.width();
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

static inline void WireNeighbourInstructions(
	lcsm::support::PointerView< lcsm::EvaluatorNode > &targetFrom,
	lcsm::support::PointerView< lcsm::EvaluatorNode > &child,
	std::vector< lcsm::Event > &events)
{
	/* Extract target and caller. */
	lcsm::EvaluatorNode *target = static_cast< lcsm::EvaluatorNode * >(child.ptr());
	lcsm::EvaluatorNode *caller = static_cast< lcsm::EvaluatorNode * >(targetFrom.ptr());
	/* Write wire's value to target. */
	lcsm::Instruction i = lcsm::CreateWriteValueInstruction(caller, target);
	lcsm::Event e = lcsm::CreateInstantEvent(std::move(i), targetFrom, child);
	events.push_back(std::move(e));
}

std::vector< lcsm::Event > lcsm::physical::Wire::invokeInstants(const lcsm::Timestamp &now)
{
	/* Wire's events adventure.
	Invoke instructions. Collect resulting databits and generate new events. Rules:
		1. To ensure optimization, we will remember the "calling" objects.
		2. After summing, we will go through all the "calling" objects and compare their value with the result of the
	sum
		- if they are different, then we will duplicate this value on the "calling" (we will create an event of the
		BroadcastValue or WriteValue instruction, where the target will be the "calling" object) according to the
	following rule: if the value cannot be forcibly written to the object (for example, Pin Input), then we will not
	create such an event with such an instruction.
		3. We will also go through all the unpassed objects and create the same instruction according to the same rule
	*/
	std::vector< lcsm::Event > events;
	std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode >, std::hash< lcsm::support::PointerView< lcsm::EvaluatorNode > > > calling;
	lcsm::support::PointerView< lcsm::EvaluatorNode > targetFrom = static_cast< lcsm::EvaluatorNode * >(this);

	/* Invoke all instructions. */
	lcsm::DataBits value = m_context->getValue();
	const lcsm::Timestamp then = m_context->lastUpdate();
	const bool takeFirst = now > then;

	/* If NOW is later, then THEN, then we should take first value as not-dirty. */
	if (takeFirst && !m_instants.empty())
	{
		lcsm::Instruction instant = m_instants.front();
		lcsm::EvaluatorNode *caller = instant.caller();
		m_instants.pop_front();
		value = caller->read();
		calling.insert({ caller });
	}

	for (lcsm::Instruction &instant : m_instants)
	{
		lcsm::EvaluatorNode *caller = instant.caller();
		value |= caller->read();
		calling.insert({ caller });
	}

	/* As C++ standard requires non-non-const iterators for std::*set, so we're forced to add all callings to
	 * vector for future traversal. */
	std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > nonConstCalling;

	/* Go through all the non-"calling" objects-neighbour and create a new events. */
	for (lcsm::support::PointerView< lcsm::EvaluatorNode > &child : m_children)
	{
		lcsm::EvaluatorNode *c = static_cast< lcsm::EvaluatorNode * >(child.ptr());
		const lcsm::support::PointerView< lcsm::EvaluatorNode > cView = c;

		/* Skip "calling". */
		const auto found = calling.find(cView);
		if (found != calling.cend())
		{
			nonConstCalling.push_back(cView);
			continue;
		}

		/* Make a new instruction for each neighbour. */
		WireNeighbourInstructions(targetFrom, child, events);
	}

	/* Go through all "calling" objects-neighbour and create a new events, where result != read. */
	for (lcsm::support::PointerView< lcsm::EvaluatorNode > &childObject : nonConstCalling)
	{
		lcsm::support::PointerView< lcsm::EvaluatorNode > child = static_cast< lcsm::EvaluatorNode * >(childObject.ptr());
		const lcsm::DataBits &neighbourDataBits = childObject->read();
		if (neighbourDataBits == value)
			continue;
		/* Make a new instruction for each neighbour. */
		WireNeighbourInstructions(targetFrom, child, events);
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
