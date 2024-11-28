#include <lcsm/Component/Identifier.h>
#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Event.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <algorithm>
#include <deque>
#include <memory>
#include <utility>
#include <vector>

lcsm::LCSMState::LCSMState(std::vector< lcsm::support::PointerView< lcsm::CGNode > > &nodes, lcsm::LCSMEngine &engine) :
	m_globalTimer(0), m_nodesView(std::addressof(nodes)), m_engineView(std::addressof(engine))
{
	for (lcsm::support::PointerView< lcsm::CGNode > &node : m_nodesView.ref())
		schedule(node);
}

lcsm::LCSMState::LCSMState(lcsm::LCSMState &&other) noexcept :
	m_globalTimer(other.m_globalTimer), m_nodesView(std::move(other.m_nodesView)), m_scheduled(std::move(other.m_scheduled))
{
}

lcsm::LCSMState &lcsm::LCSMState::operator=(lcsm::LCSMState &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::LCSMState(std::move(other)).swap(*this);
	return *this;
}

lcsm::LCSMState::~LCSMState() noexcept
{
	m_engineView->resetValues();
}

void lcsm::LCSMState::swap(lcsm::LCSMState &other) noexcept
{
	std::swap(m_globalTimer, other.m_globalTimer);
	std::swap(m_scheduled, other.m_scheduled);
}

void lcsm::LCSMState::schedulePutValue(lcsm::Identifier identifier, const lcsm::DataBits &value)
{
	m_engineView->putValue(identifier, value);
}

void lcsm::LCSMState::schedulePutValue(lcsm::Identifier identifier, lcsm::DataBits &&value)
{
	m_engineView->putValue(identifier, std::move(value));
}

void lcsm::LCSMState::schedule(lcsm::support::PointerView< lcsm::CGNode > &node)
{
	scheduleAt(node, m_globalTimer);
}

void lcsm::LCSMState::scheduleAt(support::PointerView< lcsm::CGNode > &node, timer_t timer)
{
	/* Check if queue is exists, create if needs one. */
	const auto found = m_scheduled.find(timer);
	if (found == m_scheduled.cend())
		m_scheduled[timer] = {};

	/* Invoke and generate new instructions. */
	lcsm::CGObject *object = static_cast< lcsm::CGObject * >(node.ptr());
	std::vector< lcsm::Event > events = object->invokeInstructions();

	/* Schedule events. */
	for (lcsm::Event &event : events)
		scheduleEvent(event, false, timer);
}

void lcsm::LCSMState::scheduleEvent(const lcsm::Event &event, bool isFast)
{
	scheduleEvent(event, isFast, m_globalTimer);
}

void lcsm::LCSMState::scheduleEvent(const lcsm::Event &event, bool isFast, timer_t timer)
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
		const unsigned diff = event.diff();
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
		m_globalTimer++;
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
			std::vector< lcsm::support::PointerView< lcsm::CGNode > > fastNodes;
			const std::size_t schedulerSize = scheduler.size();

			/* Extracting events from queue. */
			for (std::size_t i = 0; i < schedulerSize; i++)
			{
				/* Extract from front and remove from scheduler. */
				lcsm::Event event = scheduler.front();
				scheduler.pop_front();

				/* Targeting invoke in future node. */
				lcsm::support::PointerView< lcsm::CGNode > &target = event.targetTo();
				lcsm::Instruction &instruction = event.instruction();

				/* Check event's target's type - if it's fast, then put instruction and remember, otherwise put it to
				 * scheduler back. */
				if (target->isFast() && instruction.isBroadcast())
				{
					/* Push instruction to object's node. */
					lcsm::CGObject *object = static_cast< lcsm::CGObject * >(target.ptr());
					object->pushBackInstruction(std::move(instruction));
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
			std::vector< lcsm::support::PointerView< lcsm::CGNode > > nodes;
			const std::size_t schedulerSize = scheduler.size();

			/* Extracting events from queue. */
			for (std::size_t i = 0; i < schedulerSize; i++)
			{
				/* Extract from front and remove from scheduler. */
				lcsm::Event event = scheduler.front();
				scheduler.pop_front();

				/* Targeting invoke in future node. */
				lcsm::support::PointerView< lcsm::CGNode > &target = event.targetTo();

				/* Push instruction to object's node. */
				lcsm::Instruction &instruction = event.instruction();
				lcsm::CGObject *object = static_cast< lcsm::CGObject * >(target.ptr());
				object->pushBackInstruction(std::move(instruction));

				/* Remember this node. */
				nodes.push_back(target);
			}

			/* Invoke all nodes and push events to main scheduler. */
			mainLoop(nodes);
		}
	} while (!scheduler.empty());

	m_globalTimer++;
	m_scheduled.erase(found);
}

void lcsm::LCSMState::mainLoop(std::vector< lcsm::support::PointerView< CGNode > > &nodes, bool isFast)
{
	/* Invoke all nodes and push events to main scheduler. */
	for (lcsm::support::PointerView< lcsm::CGNode > &node : nodes)
	{
		/* Invoke and generate new instructions. */
		lcsm::CGObject *object = static_cast< lcsm::CGObject * >(node.ptr());
		std::vector< lcsm::Event > events = object->invokeInstructions();

		/* Schedule events. */
		for (lcsm::Event &event : events)
			scheduleEvent(event, isFast);
	}
}
