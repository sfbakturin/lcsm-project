#include "lcsm/IR/CG.h"
#include "lcsm/IR/Instruction.h"
#include "lcsm/Support/PointerView.hpp"
#include <lcsm/Component/Identifier.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Event.h>
#include <lcsm/Model/Width.h>
#include <unordered_set>

#include <functional>
#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::CGWire::CGWire() : lcsm::CGObject(lcsm::CGNodeType::Fast) {}

void lcsm::CGWire::reset() noexcept
{
	m_value.reset();
}

lcsm::DataBits &lcsm::CGWire::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGWire::read() const
{
	return m_value;
}

void lcsm::CGWire::write(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGWire::write(lcsm::DataBits &&value)
{
	m_value = std::move(value);
}

lcsm::model::Width lcsm::CGWire::width() const
{
	return m_value.width();
}

bool lcsm::CGWire::checkWidth(const lcsm::DataBits &value) const
{
	return value.width() == width();
}

lcsm::CGObjectType lcsm::CGWire::objectType() const noexcept
{
	return lcsm::CGObjectType::Wire;
}

lcsm::CGWire *lcsm::CGWire::asWire() noexcept
{
	return this;
}

const lcsm::CGWire *lcsm::CGWire::asWire() const noexcept
{
	return this;
}

void lcsm::CGWire::pushBackInstruction(const lcsm::Instruction &instruction)
{
	/* Only BroadcastValue and WriteValue instructions are available. */
	if (instruction.isBroadcast() || instruction.isWrite())
		lcsm::CGObject::pushBackInstruction(instruction);
	else
		throw std::logic_error("Attempt to instant non BroadcastValue for wire");
}

void lcsm::CGWire::pushBackInstruction(lcsm::Instruction &&instruction)
{
	/* Only BroadcastValue and WriteValue instructions are available. */
	if (instruction.isBroadcast() || instruction.isWrite())
		lcsm::CGObject::pushBackInstruction(std::move(instruction));
	else
		throw std::logic_error("Attempt to instant non BroadcastValue for wire");
}

static void WireNeighbourInstructions(
	lcsm::support::PointerView< lcsm::CGNode > &targetFrom,
	lcsm::support::PointerView< lcsm::CGNode > &child,
	std::vector< lcsm::Event > &events)
{
	lcsm::CGObject *target = static_cast< lcsm::CGObject * >(child.ptr());
	lcsm::CGObject *caller = static_cast< lcsm::CGObject * >(targetFrom.ptr());

	/* Make a new instruction for each neighbour. */
	switch (target->objectType())
	{
	case lcsm::CGObjectType::PinInput:
	{
		/* No instructions for input pin. */
		break;
	}
	case lcsm::CGObjectType::PinOutput:
	{
		/* Write value to output pin. */
		lcsm::Instruction i = lcsm::CreateWriteValueInstruction(caller, target);
		lcsm::Event e = lcsm::CreateInstantEvent(std::move(i), targetFrom, child);
		events.push_back(std::move(e));
		break;
	}
	case lcsm::CGObjectType::Wire:
	{
		/* Broadcast value from wire to wire. */
		lcsm::Instruction i = lcsm::CreateBroadcastValueInstruction(caller, target);
		lcsm::Event e = lcsm::CreateInstantEvent(std::move(i), targetFrom, child);
		events.push_back(std::move(e));
		break;
	}
	case lcsm::CGObjectType::Constant:
	case lcsm::CGObjectType::Power:
	case lcsm::CGObjectType::Ground:
	{
		/* No instructions for Constant/Power/Ground element. */
		break;
	}
	case lcsm::CGObjectType::Splitter:
		throw std::logic_error("Not implemented");
	case lcsm::CGObjectType::ClockInout:
		throw std::logic_error("Not implemented");
	case lcsm::CGObjectType::ClockState:
		throw std::logic_error("Not implemented");
	case lcsm::CGObjectType::TransistorBase:
		throw std::logic_error("Not implemented");
	case lcsm::CGObjectType::TransistorInout:
		throw std::logic_error("Not implemented");
	case lcsm::CGObjectType::TransistorState:
		throw std::logic_error("Not implemented");
	}
}

std::vector< lcsm::Event > lcsm::CGWire::invokeInstructions()
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
	std::unordered_set< lcsm::CGObjectView, std::hash< lcsm::CGObjectView > > calling;
	lcsm::support::PointerView< lcsm::CGNode > targetFrom = static_cast< lcsm::CGNode * >(this);

	/* Invoke all instructions. */
	lcsm::DataBits result = m_value;

	for (lcsm::Instruction &instant : m_instants)
	{
		lcsm::CGObject *caller = instant.caller();
		result |= caller->read();
		calling.insert({ caller });
	}

	/* FIXME: As C++ standard requires non-non-const iterators for std::*set, so we're forced to add all callings to
	 * vector for future traversal. */
	std::vector< lcsm::CGObjectView > nonConstCalling;

	/* Go through all the non-"calling" objects-neighbour and create a new events. */
	for (lcsm::support::PointerView< lcsm::CGNode > &child : m_children)
	{
		lcsm::CGObject *c = static_cast< lcsm::CGObject * >(child.ptr());
		const lcsm::CGObjectView cView = c;

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
	for (lcsm::CGObjectView &childObject : nonConstCalling)
	{
		lcsm::support::PointerView< lcsm::CGNode > child = static_cast< lcsm::CGNode * >(childObject.ptr());
		const lcsm::DataBits &neighbourDataBits = childObject->read();
		if (neighbourDataBits == result)
			continue;
		/* Make a new instruction for each neighbour. */
		WireNeighbourInstructions(targetFrom, child, events);
	}

	/* Save last value. */
	m_value = result;

	/* Clear instants. */
	m_instants.clear();

	return events;
}
