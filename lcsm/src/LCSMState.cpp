#include <lcsm/LCSM.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Snapshot.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <cstddef>
#include <deque>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

class SimulatorHandler
{
  public:
	SimulatorHandler(lcsm::Timestamp &timestamp);

	void addSimulatorInstruction(lcsm::Instruction &&instruction);

	bool checkPollution(std::deque< lcsm::Event > &queue, const lcsm::Snapshot &snapshot);

  private:
	lcsm::Timestamp &m_timestamp;

	lcsm::Snapshot m_polluteCircuitSnaphot;
	std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > m_polluteCircuitRequesters;
};

SimulatorHandler::SimulatorHandler(lcsm::Timestamp &timestamp) : m_timestamp(timestamp) {}

void SimulatorHandler::addSimulatorInstruction(lcsm::Instruction &&instruction)
{
	if (!instruction.isSimulatorInstruction())
	{
		throw std::logic_error("Attempt to add to simulator handler non simulator instruction!");
	}

	switch (instruction.type())
	{
	case lcsm::SimulatorInstructionType::PolluteCircuit:
	{
		m_polluteCircuitRequesters.emplace_back(instruction.caller());
		return;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Unsupported simulator's instruction found!");
}

bool SimulatorHandler::checkPollution(std::deque< lcsm::Event > &queue, const lcsm::Snapshot &snapshot)
{
	if (m_polluteCircuitRequesters.empty())
	{
		return false;
	}

	if (m_polluteCircuitSnaphot)
	{
		if (m_polluteCircuitSnaphot.equalsValues(snapshot))
		{
			return false;
		}
	}

	m_polluteCircuitSnaphot = snapshot;

	queue.clear();
	m_timestamp = m_timestamp.subnext();
	std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode > > already;

	for (lcsm::support::PointerView< lcsm::EvaluatorNode > &node : m_polluteCircuitRequesters)
	{
		if (already.find(node) != already.end())
		{
			continue;
		}
		already.insert(node);
		node->invoke(m_timestamp, queue);
	}

	m_polluteCircuitRequesters.clear();

	return true;
}

lcsm::LCSMState::LCSMState(lcsm::LCSMEngine *engine) : m_engine(engine)
{
	using It = std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > >;

	for (const It &object : m_engine->m_objects)
	{
		const lcsm::Identifier id = object.first;
		const std::shared_ptr< lcsm::EvaluatorNode > &obj = object.second;

		// Create context and setup physical object to new state.
		obj->setContext(m_snapshot.allocate(id, obj->contextSize(), obj->privateContextSize()));
	}

	// Initialize all inputs.
	for (It &input : m_engine->m_realInputs)
	{
		const lcsm::support::PointerView< lcsm::EvaluatorNode > node = input.second;
		m_roots.push_back(node);
	}

	// Initialize all roots.
	for (It &root : m_engine->m_realRoots)
	{
		const lcsm::support::PointerView< lcsm::EvaluatorNode > node = root.second;
		m_roots.push_back(node);
	}
}

lcsm::LCSMState::LCSMState(const lcsm::LCSMState &other) :
	m_timestamp(other.m_timestamp), m_queue(other.m_queue), m_snapshot(other.m_snapshot), m_roots(other.m_roots),
	m_engine(other.m_engine)
{
}

lcsm::LCSMState::LCSMState(lcsm::LCSMState &&other) noexcept :
	m_timestamp(std::move(other.m_timestamp)), m_queue(std::move(other.m_queue)),
	m_snapshot(std::move(other.m_snapshot)), m_roots(std::move(other.m_roots)), m_engine(std::move(other.m_engine))
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
	std::swap(m_timestamp, other.m_timestamp);
	std::swap(m_queue, other.m_queue);
	std::swap(m_snapshot, other.m_snapshot);
	std::swap(m_roots, other.m_roots);
	std::swap(m_engine, other.m_engine);
}

std::size_t lcsm::LCSMState::valueSize(lcsm::Identifier id) const
{
	/* Fidn and return context's size. */;
	return m_snapshot.valueSize(id);
}

const lcsm::DataBits &lcsm::LCSMState::valueOf(lcsm::Identifier id, std::size_t i) const
{
	/* Find and return i's value from context. */
	return m_snapshot.valueOf(id, i);
}

const std::vector< lcsm::DataBits > &lcsm::LCSMState::valuesOf(lcsm::Identifier id) const
{
	/* Find and return values from context. */
	return m_snapshot.valuesOf(id);
}

void lcsm::LCSMState::putValue(lcsm::Identifier id, const lcsm::DataBits &databits, std::size_t i)
{
	/* Find value by identifier. */
	lcsm::support::PointerView< lcsm::Context > context = m_snapshot.at(id);

	/* Check if id's object is input. */
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > >::iterator foundInp =
		m_engine->m_realInputs.find(id);
	if (foundInp == m_engine->m_realInputs.end())
	{
		throw std::logic_error("No such input with identifier found");
	}

	/* Check size of value. */
	if (i >= context->size())
	{
		throw std::out_of_range("Context: Index out of bound");
	}

	/* Update value. */
	context->updateValue(i, databits, m_timestamp, true);

	/* Verify value. */
	foundInp->second->verifyContext();
}

void lcsm::LCSMState::putValue(lcsm::Identifier id, std::initializer_list< lcsm::DataBits > databits)
{
	/* Find value by identifier. */
	lcsm::support::PointerView< lcsm::Context > context = m_snapshot.at(id);

	/* Check if id's object is input. */
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > >::const_iterator foundInp =
		m_engine->m_realInputs.find(id);
	if (foundInp == m_engine->m_realInputs.cend())
	{
		throw std::logic_error("No such input with identifier found");
	}

	/* Check size of value. */
	if (databits.size() != context->size())
	{
		throw std::out_of_range("Context: Index out of bound");
	}

	/* Update values. */
	context->updateValues(m_timestamp, databits);

	/* Verify values. */
	foundInp->second->verifyContext();
}

void lcsm::LCSMState::ticks(unsigned n)
{
	for (unsigned i = 0; i < n; i++)
	{
		tick();
	}
}

template< typename T >
static inline bool IsPresentedImpl(const std::unordered_set< lcsm::support::PointerView< T > > &set, const T *tgt)
{
	return std::find_if(set.begin(),
						set.end(),
						[tgt](const lcsm::support::PointerView< T > &elm) noexcept { return elm == tgt; }) != set.end();
}

void lcsm::LCSMState::tick()
{
	/* Tick. */
	m_timestamp++;
	lcsm::Timestamp now = m_timestamp;

	/* At each global tick (or: "clock") of the circuit we perform the following actions: 1. Generate new events from
	 * the root elements of the circuit and write them to the found or created new queue for the current step. 2. Go to
	 * the main loop (described below). */
	std::deque< lcsm::Event > queue;

	std::unordered_map< lcsm::Timestamp, std::deque< lcsm::Event > >::iterator found = m_queue.find(m_timestamp);
	if (found != m_queue.end())
	{
		queue = std::move(found->second);
		m_queue.erase(found);
	}

	for (lcsm::support::PointerView< lcsm::EvaluatorNode > &root : m_roots)
	{
		/* Invoke and generate new instructions. */
		root->invoke(now, queue);
	}

	/* Main queue and visited elements. Generally, we should traverse over all elements only once to ensure, that
	 * there is no looping in wires or elements. */
	bool loop = true;
	std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode > > visited;

	/* Simulator handler. */
	SimulatorHandler sm = now;

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
lMainLoop:
		/* Priority steps. */
		for (lcsm::target_t nodeType = lcsm::NodeType::FirstNodeType; nodeType < lcsm::NodeType::LastNodeType; nodeType++)
		{
			/* Loop and invoke all possible specific typed nodes. */
			for (;;)
			{
				std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > nodes;
				const std::size_t size = queue.size();

				/* Find the minimum node type to ensure if everything is found. */
				for (const lcsm::Event &event : queue)
				{
					/* Targeting invoke in future node. */
					const lcsm::Instruction &instruction = event.instruction();
					const lcsm::EvaluatorNode *caller = instruction.caller();
					const lcsm::EvaluatorNode *target = instruction.target();

					/* Check if caller is not in visited set. */
					if (IsPresentedImpl(visited, caller))
					{
						continue;
					}

					/* Find the min of real type and target's type. */
					const lcsm::target_t targetNodeType = target->nodeType();
					nodeType = std::min(nodeType, targetNodeType);
				}

				/* Extract all events from queue. */
				for (std::size_t i = 0; i < size && !queue.empty(); i++)
				{
					/* Extract from front and remove from scheduler. */
					lcsm::Event event = queue.front();
					queue.pop_front();

					/* Targeting invoke in future node. */
					lcsm::Instruction &instruction = event.instruction();
					const lcsm::support::PointerView< lcsm::EvaluatorNode > caller = instruction.caller();
					lcsm::support::PointerView< lcsm::EvaluatorNode > target = instruction.target();

					/* Check if caller is not in visited set. */
					if (IsPresentedImpl(visited, caller.get()))
					{
						queue.push_back(std::move(event));
						continue;
					}

					/* Check event's target's type - if it's the one, then put instruction and remember, otherwise
					 * put it to scheduler back. */
					const lcsm::target_t targetNodeType = target->nodeType();
					if (targetNodeType == nodeType)
					{
						/* Push instruction to object's node. */
						target->add(std::move(instruction));
						/* Remember this node. */
						nodes.push_back(target);
						/* Find all events, where caller is certain this one. */
						for (std::size_t j = i; j < size && !queue.empty(); j++)
						{
							/* Extract from front and remove from scheduler. */
							lcsm::Event otherEvent = queue.front();
							queue.pop_front();

							/* Targeting invoke in future node. */
							lcsm::Instruction &otherInstruction = otherEvent.instruction();
							const lcsm::support::PointerView< lcsm::EvaluatorNode > otherCaller = otherInstruction.caller();
							lcsm::support::PointerView< lcsm::EvaluatorNode > otherTarget = otherInstruction.target();

							/* Add to nodes, if caller is presented. */
							if (caller == otherCaller && otherTarget->nodeType() == targetNodeType)
							{
								otherTarget->add(std::move(otherInstruction));
								nodes.push_back(otherTarget);
							}
							else
							{
								/* If not, then return to queue. */
								queue.push_back(std::move(otherEvent));
							}
						}
						/* Mark as visited. */
						visited.insert(caller);
					}
					else
					{
						/* If non specific node is high as priority, then we should set iterator to begin. */
						queue.push_back(std::move(event));
					}
				}

				/* Break looping, when there is no nodes. */
				if (nodes.empty())
				{
					break;
				}

				/* Invoke all nodes and push events to main scheduler, skip already invoked. */
				std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode > > already;
				for (lcsm::support::PointerView< lcsm::EvaluatorNode > &node : nodes)
				{
					/* If already executed, then do not invoke. */
					if (already.find(node) != already.end())
					{
						continue;
					}

					/* Make as already visited. */
					already.insert(node);

					/* Invoke and generate new instructions. */
					std::deque< lcsm::Event > events = node->invoke(now);

					/* Schedule events. */
					for (lcsm::Event &event : events)
					{
						lcsm::Instruction &instruction = event.instruction();

						/* Filter event. */
						if (instruction.isSimulatorInstruction())
						{
							sm.addSimulatorInstruction(std::move(instruction));
						}
						else if (event.isInstant())
						{
							queue.push_back(std::move(event));
						}
						else if (event.isFuture())
						{
							const lcsm::Timestamp T = event.diff();
							if (m_queue.find(T) == m_queue.end())
							{
								m_queue[T] = {};
							}
							m_queue[T].push_back(std::move(event));
						}
					}
				}
			}
		}

		/* Clear all visited. */
		visited.clear();

		/* Case: scheduler is empty. */
		if (queue.empty())
		{
			/* Check the simulator's hander. */
			if (sm.checkPollution(queue, m_snapshot))
			{
				goto lMainLoop;
			}

			break;
		}

		/* Case: stabilized circuit. */
		{
			/* Copy all contents of contexts and events. */
			lcsm::Snapshot copySnapshot = m_snapshot;
			std::deque< lcsm::Event > copyEvents = queue;

			/* Extract all instants and add them to objects. */
			const std::size_t size = queue.size();
			std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > nodes;
			for (std::size_t i = 0; i < size; i++)
			{
				/* Extract from front and remove from scheduler. */
				lcsm::Event event = queue.front();
				queue.pop_front();

				/* Add scheduled instant to target. */
				lcsm::Instruction instruction = event.instruction();
				lcsm::support::PointerView< lcsm::EvaluatorNode > target = instruction.target();
				target->add(std::move(instruction));
				nodes.push_back(std::move(target));
			}

			/* Invoke all instants. */
			for (lcsm::support::PointerView< lcsm::EvaluatorNode > &node : nodes)
			{
				node->invoke(now);
			}

			/* Compare contexts. */
			const bool equals = copySnapshot.equalsStrict(m_snapshot);

			/* Fixup contexts. */
			for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > > &object : m_engine->m_objects)
			{
				const lcsm::Identifier id = object.first;
				lcsm::EvaluatorNode *obj = object.second.get();
				obj->setContext(copySnapshot.at(id));
			}
			m_snapshot = std::move(copySnapshot);

			/* If contexts are equals to each other, then circuit is now stabilized: need to clear current scheduler and
			 * stop this step. Otherwise, fixup scheduled and continue this loop. */
			if (equals)
			{
				loop = sm.checkPollution(queue, m_snapshot);
			}
			else
			{
				queue = std::move(copyEvents);
			}
		}
	} while (loop);

	/* Traverse all contexts and remove pollution. */
	m_snapshot.clearPollution();
}
