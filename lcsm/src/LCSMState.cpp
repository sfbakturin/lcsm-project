#include <lcsm/LCSM.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <algorithm>
#include <cstddef>
#include <deque>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::LCSMState::LCSMState(lcsm::LCSMEngine *engine) : m_enginePtr(engine)
{
	for (auto &object : m_enginePtr->m_objects)
	{
		const lcsm::Identifier &id = object.first;
		std::shared_ptr< lcsm::EvaluatorNode > &obj = object.second;

		// Create context and setup physical object to new state.
		m_contexts[id] = lcsm::Context(obj->contextSize(), obj->privateContextSize());
		const lcsm::support::PointerView< lcsm::Context > context = std::addressof(m_contexts[id]);
		obj->setContext(context);
	}

	// Initialize all inputs and roots.
	for (auto &input : m_enginePtr->m_realInputs)
	{
		lcsm::support::PointerView< lcsm::EvaluatorNode > node = input.second;
		m_roots.push_back(node);
	}

	for (auto &root : m_enginePtr->m_realRoots)
	{
		lcsm::support::PointerView< lcsm::EvaluatorNode > node = root.second;
		m_roots.push_back(node);
	}
}

lcsm::LCSMState::LCSMState(const lcsm::LCSMState &other) :
	m_globalTimer(other.m_globalTimer), m_globalQueue(other.m_globalQueue), m_contexts(other.m_contexts),
	m_roots(other.m_roots), m_enginePtr(other.m_enginePtr)
{
}

lcsm::LCSMState::LCSMState(lcsm::LCSMState &&other) noexcept :
	m_globalTimer(std::move(other.m_globalTimer)), m_globalQueue(std::move(other.m_globalQueue)),
	m_contexts(std::move(other.m_contexts)), m_roots(std::move(other.m_roots)), m_enginePtr(std::move(other.m_enginePtr))
{
}

lcsm::LCSMState &lcsm::LCSMState::operator=(const lcsm::LCSMState &other)
{
	return lcsm::support::CopyAssign< lcsm::LCSMState >(this, other);
}

lcsm::LCSMState &lcsm::LCSMState::operator=(lcsm::LCSMState &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::LCSMState >(this, std::move(other));
}

void lcsm::LCSMState::swap(lcsm::LCSMState &other) noexcept
{
	std::swap(m_globalTimer, other.m_globalTimer);
	std::swap(m_globalQueue, other.m_globalQueue);
	std::swap(m_contexts, other.m_contexts);
	std::swap(m_roots, other.m_roots);
	std::swap(m_enginePtr, other.m_enginePtr);
}

std::size_t lcsm::LCSMState::valueSize(lcsm::Identifier id) const
{
	/* Find value by identifier. */
	const std::unordered_map< lcsm::Identifier, lcsm::Context >::const_iterator found = m_contexts.find(id);

	/* Check if exists. */
	if (found == m_contexts.end())
		throw std::logic_error("No such value with identifier found");

	/* Return context's size. */
	const lcsm::Context &context = found->second;
	return context.size();
}

const lcsm::DataBits &lcsm::LCSMState::valueOf(lcsm::Identifier id, std::size_t i) const
{
	const std::unordered_map< lcsm::Identifier, lcsm::Context >::const_iterator found = m_contexts.find(id);

	/* Check if object with specified identifier exists. */
	if (found == m_contexts.cend())
		throw std::logic_error("LCSM State: object with specified identifier is not found.");

	/* Return i's value from context. */
	return found->second.getValue(i);
}

const std::vector< lcsm::DataBits > &lcsm::LCSMState::valuesOf(lcsm::Identifier id) const
{
	const std::unordered_map< lcsm::Identifier, lcsm::Context >::const_iterator found = m_contexts.find(id);

	/* Check if object with specified identifier exists. */
	if (found == m_contexts.cend())
		throw std::logic_error("LCSM State: object with specified identifier is not found.");

	/* Return values from context. */
	return found->second.values();
}

void lcsm::LCSMState::putValue(lcsm::Identifier id, const lcsm::DataBits &databits, std::size_t i)
{
	/* Find value by identifier. */
	std::unordered_map< lcsm::Identifier, lcsm::Context >::iterator foundIt = m_contexts.find(id);

	/* Check if exists. */
	if (foundIt == m_contexts.end())
		throw std::logic_error("No such value with identifier found");

	/* Check if id's object is input. */
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > >::const_iterator foundInp =
		m_enginePtr->m_realInputs.find(id);
	if (foundInp == m_enginePtr->m_realInputs.cend())
		throw std::logic_error("No such input with identifier found");

	/* Check size of value. */
	lcsm::Context &context = foundIt->second;
	if (i >= context.size())
		throw std::out_of_range("Context: Index out of bound");

	/* Update value. */
	context.beginUpdate(m_globalTimer);
	context.updateValue(i, databits);
	context.endUpdate();
}

void lcsm::LCSMState::putValue(lcsm::Identifier id, std::initializer_list< lcsm::DataBits > databits)
{
	/* Find value by identifier. */

	/* Check if exists. */
	std::unordered_map< lcsm::Identifier, lcsm::Context >::iterator foundIt = m_contexts.find(id);
	if (foundIt == m_contexts.end())
		throw std::logic_error("No such value with identifier found");

	/* Check if id's object is input. */
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > >::const_iterator foundInp =
		m_enginePtr->m_realInputs.find(id);
	if (foundInp == m_enginePtr->m_realInputs.cend())
		throw std::logic_error("No such input with identifier found");

	/* Check size of value. */
	lcsm::Context &context = foundIt->second;
	if (databits.size() != context.size())
		throw std::out_of_range("Context: Index out of bound");

	/* Update values. */
	context.updateValues(m_globalTimer, databits);
}

void lcsm::LCSMState::scheduleEvent(const lcsm::Event &event)
{
	scheduleEvent(event, m_globalTimer);
}

void lcsm::LCSMState::scheduleEvent(const lcsm::Event &event, lcsm::Timestamp timestamp)
{
	/* If scheduling event is instant, then, we should instant it in this timestamp. */
	if (event.isInstant())
	{
		/* Ensure queue existence. */
		if (m_globalQueue.find(timestamp) == m_globalQueue.end())
			m_globalQueue[timestamp] = {};
		/* Add new event to queue. */
		m_globalQueue[timestamp].push_back(event);
		return;
	}

	/* Extract instant event from future with time difference. */
	const Timestamp diff = event.diff();
	lcsm::Event instant = event.toInstant();

	/* Shift timer. */
	timestamp += diff;

	/* Check if queue is exists, create if needs one. */
	if (m_globalQueue.find(timestamp) == m_globalQueue.end())
		m_globalQueue[timestamp] = {};

	/* Add new event to queue. */
	m_globalQueue[timestamp].push_back(std::move(instant));
}

void lcsm::LCSMState::ticks(unsigned n)
{
	for (unsigned i = 0; i < n; i++)
		tick();
}

void lcsm::LCSMState::tick()
{
	/* Tick. */
	m_globalTimer++;

	/* At each global tick (or: "clock") of the circuit we perform the following actions: 1. Generate new events from
	 * the root elements of the circuit and write them to the found or created new queue for the current step. 2. Go to
	 * the main loop (described below). */
	std::deque< lcsm::Event > localQueue;

	auto found = m_globalQueue.find(m_globalTimer);
	if (found != m_globalQueue.end())
	{
		localQueue = std::move(found->second);
		m_globalQueue.erase(found);
	}

	for (lcsm::support::PointerView< lcsm::EvaluatorNode > &root : m_roots)
	{
		/* Invoke and generate new instructions. */
		std::vector< lcsm::Event > events = root->invokeInstants(m_globalTimer);

		/* Schedule events. */
		for (lcsm::Event &event : events)
			localQueue.push_back(event);
	}

	/* Main queue and visited elements. Generally, we should traverse over all elements only once to ensure, that
	 * there is no looping in wires or elements. */
	bool loop = true;
	std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > visited;

	/* Main looping mini-steps for step by timeouts and sync. To begin with, we execute all our scheduled instructions
	 * in the following order: while it is possible to execute instructions smaller than this node type (in order of
	 * execution speed) and this object has never participated in execution, we continue our cycle and save new events
	 * in the queue. At the moment when all physical elements of our scheme have been executed (a cycle has appeared) or
	 * all scheduled events have been executed, we move on to the next stage. Here we check the scheme for
	 * stabilization: we create a snapshot of our context values, execute instructions on imaginary values ​​and if
	 * they have not changed with the old ones, we finish the main loop.
	 */
	do
	{
		/* Priority steps. */
		for (lcsm::target_t nodeType = lcsm::NodeType::FirstNodeType; nodeType < lcsm::NodeType::LastNodeType; nodeType++)
		{
			/* Loop and invoke all possible specific typed nodes. */
			do
			{
				std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > nodes;
				const std::size_t size = localQueue.size();

				/* Extract all events from queue. */
				for (std::size_t i = 0; i < size && !localQueue.empty(); i++)
				{
					/* Extract from front and remove from scheduler. */
					lcsm::Event event = localQueue.front();
					localQueue.pop_front();

					/* Targeting invoke in future node. */
					lcsm::Instruction &instruction = event.instruction();
					lcsm::support::PointerView< lcsm::EvaluatorNode > caller = instruction.caller();
					lcsm::support::PointerView< lcsm::EvaluatorNode > target = instruction.target();

					/* Check if target is not in visited set. */
					if (std::find(visited.begin(), visited.end(), caller) != visited.end())
					{
						localQueue.push_back(event);
						continue;
					}

					/* Check event's target's type - if it's the one, then put instruction and remember, otherwise
					 * put it to scheduler back. */
					const lcsm::target_t targetNodeType = target->nodeType();
					if (targetNodeType == nodeType)
					{
						/* Push instruction to object's node. */
						target->addInstant(instruction);
						/* Remember this node. */
						nodes.push_back(target);
						/* Find all events, where caller is certain this one. */
						for (std::size_t j = i; j < size && !localQueue.empty(); j++)
						{
							/* Extract from front and remove from scheduler. */
							lcsm::Event otherEvent = localQueue.front();
							localQueue.pop_front();

							/* Targeting invoke in future node. */
							lcsm::Instruction &otherInstruction = otherEvent.instruction();
							lcsm::support::PointerView< lcsm::EvaluatorNode > otherCaller = otherInstruction.caller();
							lcsm::support::PointerView< lcsm::EvaluatorNode > otherTarget = otherInstruction.target();

							/* Add to nodes, if caller is presented. */
							if (caller == otherCaller)
							{
								otherTarget->addInstant(otherInstruction);
								nodes.push_back(otherTarget);
							}
							else
							{
								/* If not, then return to queue. */
								localQueue.push_back(otherEvent);
							}
						}
						/* Mark as visited. */
						visited.push_back(caller);
					}
					else
					{
						/* If non specific node is high as priority, then we should set iterator to begin. */
						localQueue.push_back(event);
					}
				}

				/* Break looping, when there is no nodes. */
				if (nodes.empty())
					break;

				/* Invoke all nodes and push events to main scheduler. */
				for (lcsm::support::PointerView< lcsm::EvaluatorNode > &node : nodes)
				{
					/* Invoke and generate new instructions. */
					std::vector< lcsm::Event > events = node->invokeInstants(m_globalTimer);

					/* Schedule events. */
					for (lcsm::Event &event : events)
						localQueue.push_back(event);
				}
			} while (true);
		}

		/* Case: scheduler is empty. No need to double check something. */
		if (localQueue.empty())
			break;

		/* Clear all visited. */
		visited.clear();

		/* Case: stabilized circuit. */
		{
			/* Copy all contents of contexts and events. */
			std::unordered_map< lcsm::Identifier, lcsm::Context > copyContexts = m_contexts;
			std::deque< lcsm::Event > copyEvents = localQueue;

			/* Extract all instants and add them to objects. */
			const std::size_t size = localQueue.size();
			std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > nodes;
			for (std::size_t i = 0; i < size; i++)
			{
				/* Extract from front and remove from scheduler. */
				lcsm::Event event = localQueue.front();
				localQueue.pop_front();

				/* Add scheduled instant to target. */
				lcsm::Instruction instruction = event.instruction();
				lcsm::support::PointerView< lcsm::EvaluatorNode > target = instruction.target();
				target->addInstant(std::move(instruction));
				nodes.push_back(std::move(target));
			}

			/* Invoke all instants. */
			for (lcsm::support::PointerView< lcsm::EvaluatorNode > &node : nodes)
			{
				node->invokeInstants(m_globalTimer);
			}

			/* Compare contexts. */
			bool equals = true;
			for (std::unordered_map< lcsm::Identifier, lcsm::Context >::const_iterator it = m_contexts.cbegin();
				 it != m_contexts.cend();
				 it++)
			{
				const lcsm::Identifier &id = it->first;
				const lcsm::Context &lhs = it->second;
				const lcsm::Context &rhs = copyContexts[id];
				if (!lhs.isEqualsValues(rhs))
				{
					equals = false;
					break;
				}
			}

			/* Fixup contexts. */
			for (auto &context : copyContexts)
			{
				const lcsm::Identifier &id = context.first;
				lcsm::Context &ctx = context.second;
				m_enginePtr->m_objects[id]->setContext({ ctx });
			}
			m_contexts = std::move(copyContexts);

			/* If contexts are equals to each other, then circuit is now stabilized: need to clear current scheduler and
			 * stop this step. Otherwise, fixup scheduled and continue this loop. */
			if (equals)
			{
				loop = false;
				localQueue.clear();
			}
			else
			{
				localQueue = std::move(copyEvents);
			}
		}
	} while (loop);
}
