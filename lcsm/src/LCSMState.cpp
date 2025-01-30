#include <lcsm/LCSM.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <algorithm>
#include <deque>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::LCSMState::LCSMState(std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > > &objects)
{
	for (auto &object : objects)
	{
		const lcsm::Identifier &id = object.first;
		std::shared_ptr< lcsm::EvaluatorNode > &obj = object.second;
		const lcsm::ObjectType objectType = obj->objectType();
		switch (objectType)
		{
		case UnknownObjectType:
			throw std::logic_error("State can't have a deal with unknown object type.");
		case PureInt:
		{
			// For pure internal objects there is no
			break;
		}
		case RootInt:
		case IntExtIn:
		{
			// For all roots and inputs we prefer to schedule new value traversals.
			lcsm::support::PointerView< lcsm::EvaluatorNode > node = obj;
			m_roots.push_back(node);
			schedule(node);
			break;
		}
		case IntExtOut:
		{
			break;
		}
		case LastObjectType:
			break;
		}
		// TODO: Simplify it somehow.
		// if (obj->isConstant())
		// {
		// 	// TODO: Add error handling, where there is no such identifier.
		// 	const lcsm::DataBits &databits = constants.at(id);
		// 	// TODO: Add error handling, when there is found duplicate identifier.
		// 	m_values[id] = { m_globalTimer, databits };
		// }
		// else
		// {
		// 	// TODO: Add error handling, when there is found duplicate identifier.
		// 	m_values[id] = lcsm::Value{ m_globalTimer };
		// }
		// if (obj->isPinInput() || obj->isConstant())	   // TODO: Simplify (!!!).
		// {
		// 	lcsm::support::PointerView< lcsm::CGNode > node = static_cast< lcsm::CGNode * >(obj.get());
		// 	m_roots.push_back(node);
		// 	schedule(node);
		// }
	}
}

lcsm::LCSMState::LCSMState(const lcsm::LCSMState &other) :
	m_globalTimer(other.m_globalTimer), m_scheduled(other.m_scheduled), m_values(other.m_values)
{
}

lcsm::LCSMState::LCSMState(lcsm::LCSMState &&other) noexcept :
	m_globalTimer(std::move(other.m_globalTimer)), m_scheduled(std::move(other.m_scheduled)), m_values(std::move(other.m_values))
{
}

lcsm::LCSMState &lcsm::LCSMState::operator=(const lcsm::LCSMState &other)
{
	if (this != std::addressof(other))
		lcsm::LCSMState(other).swap(*this);
	return *this;
}

lcsm::LCSMState &lcsm::LCSMState::operator=(lcsm::LCSMState &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::LCSMState(std::move(other)).swap(*this);
	return *this;
}

void lcsm::LCSMState::swap(lcsm::LCSMState &other) noexcept
{
	std::swap(m_globalTimer, other.m_globalTimer);
	std::swap(m_scheduled, other.m_scheduled);
	std::swap(m_values, other.m_values);
}

const lcsm::DataBits &lcsm::LCSMState::valueOf(lcsm::Identifier identifier) const
{
	const lcsm::Context &value = m_values.at(identifier);
	return value.getValue();
}

void lcsm::LCSMState::putValue(lcsm::Identifier identifier, const lcsm::DataBits &databits)
{
	// Find value by idenitifer.
	auto foundIt = m_values.find(identifier);
	if (foundIt == m_values.end())
		throw std::logic_error("No such value with identifier found");

	// Tick at as user to "pollute" the circuit.
	m_globalTimer = m_globalTimer.tickUser();

	// Update value.
	foundIt->second.updateValue(m_globalTimer, databits);
}

void lcsm::LCSMState::putValue(lcsm::Identifier identifier, lcsm::DataBits &&databits)
{
	// Find value by idenitifer.
	auto foundIt = m_values.find(identifier);
	if (foundIt == m_values.end())
		throw std::logic_error("No such value with identifier found");

	// Tick at as user to "pollute" the circuit.
	m_globalTimer = m_globalTimer.tickUser();

	// Update value.
	foundIt->second.updateValue(m_globalTimer, std::move(databits));
}

void lcsm::LCSMState::schedule(lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	scheduleAt(node, m_globalTimer);
}

void lcsm::LCSMState::scheduleAt(support::PointerView< lcsm::EvaluatorNode > &node, lcsm::Timestamp timer)
{
	/* Check if queue is exists, create if needs one. */
	const auto found = m_scheduled.find(timer);
	if (found == m_scheduled.cend())
		m_scheduled[timer] = {};

	/* Invoke and generate new instructions. */
	lcsm::EvaluatorNode *object = static_cast< lcsm::EvaluatorNode * >(node.ptr());
	std::vector< lcsm::Event > events = object->invokeInstants(m_globalTimer);	  // FIXME

	/* Schedule events. */
	for (lcsm::Event &event : events)
		scheduleEvent(event, false, timer);
}

void lcsm::LCSMState::scheduleEvent(const lcsm::Event &event, bool isFast)
{
	scheduleEvent(event, isFast, m_globalTimer);
}

void lcsm::LCSMState::scheduleEvent(const lcsm::Event &event, bool isFast, Timestamp timer)
{
	if (event.isInstant())
	{
		if (isFast)
			m_scheduled[timer].push_front(event);
		else
			m_scheduled[timer].push_back(event);
	}
	else
	{
		/* Extract instant event from future with time difference. */
		const Timestamp diff = event.diff();
		lcsm::Event instant = event.toInstant();

		/* Shift timer. */
		timer += diff;

		/* Check if queue is exists, create if needs one. */
		const auto found = m_scheduled.find(timer);
		if (found == m_scheduled.cend())
			m_scheduled[timer] = {};

		if (isFast)
			m_scheduled[timer].push_front(std::move(instant));
		else
			m_scheduled[timer].push_back(std::move(instant));
	}
}

void lcsm::LCSMState::step(unsigned n)
{
	for (unsigned i = 0; i < n; i++)
		stepOnce();
}

void lcsm::LCSMState::stepOnce()
{
	/* If not scheduled, than no changing timer. */
	if (m_scheduled.empty())
		return;

	/* Find scheduled by timer, if found continue. */
	const auto found = m_scheduled.find(m_globalTimer);
	if (found == m_scheduled.cend())
	{
		// m_scheduled.begin() // FIXME
		return;
	}

	/* Main queue. */
	std::deque< lcsm::Event > &scheduler = found->second;

	/* Main looping mini-steps for step by timeouts and sync. */
	do
	{
		/* Loop and invoke all possible fast nodes (Wires). */
		do
		{
			std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > fastNodes;
			const std::size_t schedulerSize = scheduler.size();

			/* Extracting events from queue. */
			for (std::size_t i = 0; i < schedulerSize; i++)
			{
				/* Extract from front and remove from scheduler. */
				lcsm::Event event = scheduler.front();
				scheduler.pop_front();

				/* Targeting invoke in future node. */
				lcsm::support::PointerView< lcsm::EvaluatorNode > &target = event.targetTo();
				lcsm::Instruction &instruction = event.instruction();

				/* Check event's target's type - if it's fast, then put instruction and remember, otherwise put it to
				 * scheduler back. */
				if (target->nodeType() == lcsm::NodeType::Fast)
				{
					/* Push instruction to object's node. */
					lcsm::EvaluatorNode *object = static_cast< lcsm::EvaluatorNode * >(target.ptr());
					object->addInstant(std::move(instruction));
					/* Remember this node. */
					fastNodes.push_back(target);
				}
				else
				{
					/* Return back node. */
					scheduler.push_back(std::move(event));
				}
			}

			/* Break looping, when there is no fast nodes. */
			if (fastNodes.empty())
				break;

			/* Invoke all fast nodes and push events to main scheduler. */
			mainLoop(fastNodes, true);
		} while (true);

		/* Loop and invoke all static/dynamic/composite nodes. */
		{
			std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > nodes;
			const std::size_t schedulerSize = scheduler.size();

			/* Extracting events from queue. */
			for (std::size_t i = 0; i < schedulerSize; i++)
			{
				/* Extract from front and remove from scheduler. */
				lcsm::Event event = scheduler.front();
				scheduler.pop_front();

				/* Targeting invoke in future node. */
				lcsm::support::PointerView< lcsm::EvaluatorNode > &target = event.targetTo();

				/* Push instruction to object's node. */
				lcsm::Instruction &instruction = event.instruction();
				lcsm::EvaluatorNode *object = static_cast< lcsm::EvaluatorNode * >(target.ptr());
				object->addInstant(std::move(instruction));

				/* Remember this node. */
				nodes.push_back(target);
			}

			/* Invoke all nodes and push events to main scheduler. */
			mainLoop(nodes);
		}
	} while (!scheduler.empty());

	// m_globalTimer++; // FIXME
	m_scheduled.erase(found);
}

void lcsm::LCSMState::mainLoop(std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > &nodes, bool isFast)
{
	/* Invoke all nodes and push events to main scheduler. */
	for (lcsm::support::PointerView< lcsm::EvaluatorNode > &node : nodes)
	{
		/* Invoke and generate new instructions. */
		lcsm::EvaluatorNode *object = static_cast< lcsm::EvaluatorNode * >(node.ptr());
		std::vector< lcsm::Event > events = object->invokeInstants(m_globalTimer);

		/* Schedule events. */
		for (lcsm::Event &event : events)
			scheduleEvent(event, isFast);
	}
}
