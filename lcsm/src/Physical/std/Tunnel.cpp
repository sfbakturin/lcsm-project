#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/std/Tunnel.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_set>

#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Tunnel::Tunnel(lcsm::object_type_t objectType) : lcsm::EvaluatorNode(objectType) {}

lcsm::NodeType lcsm::physical::Tunnel::nodeType() const noexcept
{
	return lcsm::NodeType::Fast;
}

std::size_t lcsm::physical::Tunnel::contextSize() const noexcept
{
	return 1;
}

std::size_t lcsm::physical::Tunnel::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Tunnel::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	// If context already exists, reset it.
	if (m_context)
	{
		resetContext();
	}

	// Set and verify context.
	m_context = context;
	verifyContext();
}

void lcsm::physical::Tunnel::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Tunnel::verifyContext()
{
	// Check global sizes.
	if (m_context->size() != contextSize() || m_context->privateSize() != privateContextSize())
	{
		resetContext();
		throw std::logic_error("Bad context size!");
	}
}

void lcsm::physical::Tunnel::addInstant(const lcsm::Instruction &instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::InstructionType type = instruction.type();
	if (m_wiring == caller && type == lcsm::InstructionType::WriteValue)
	{
		m_instantsWiring.push_back(instruction);
	}
	else if (type == lcsm::InstructionType::WriteValue)
	{
		m_instantsWiring.push_back(instruction);
	}
	else
	{
		throw std::logic_error("Bad instruction!");
	}
}

void lcsm::physical::Tunnel::addInstant(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::InstructionType type = instruction.type();
	if (m_wiring == caller && type == lcsm::InstructionType::WriteValue)
	{
		m_instantsWiring.push_back(std::move(instruction));
	}
	else if (type == lcsm::InstructionType::WriteValue)
	{
		m_instantsWiring.push_back(std::move(instruction));
	}
	else
	{
		throw std::logic_error("Bad instruction!");
	}
}

static inline void
	CreateInstr(lcsm::EvaluatorNode *targetFrom, lcsm::EvaluatorNode *targetTo, const lcsm::DataBits &value, std::vector< lcsm::Event > &events)
{
	// Write wire's value to target.
	lcsm::Instruction i = lcsm::CreateWriteValueInstruction(targetFrom, targetTo, value);
	events.emplace_back(std::move(i));
}

std::vector< lcsm::Event > lcsm::physical::Tunnel::invokeInstants(const lcsm::Timestamp &now)
{
	// Created events.
	std::vector< lcsm::Event > events;

	// Extract contexted value.
	lcsm::DataBits value = m_context->getValue();
	const lcsm::Timestamp &then = m_context->lastUpdate();
	const bool takeFirst = now > then;

	// If NOW is later, then THEN, then we should take first value as not-dirty.
	if (takeFirst)
	{
		const bool hasWiring = !m_instantsWiring.empty();
		const bool hasTunnel = !m_instantsTunnel.empty();
		if (hasWiring && hasTunnel)
		{
			const lcsm::Instruction instantWiring = m_instantsWiring.front();
			m_instantsWiring.pop_front();
			value = instantWiring.value();
			const lcsm::Instruction instantTunnel = m_instantsTunnel.front();
			m_instantsTunnel.pop_front();
			value |= instantTunnel.value();
		}
		else if (hasWiring && !hasTunnel)
		{
			const lcsm::Instruction instantWiring = m_instantsWiring.front();
			m_instantsWiring.pop_front();
			value = instantWiring.value();
		}
		else if (!hasWiring && hasTunnel)
		{
			const lcsm::Instruction instantTunnel = m_instantsTunnel.front();
			m_instantsTunnel.pop_front();
			value = instantTunnel.value();
		}
	}

	// Invoke all instructions.
	std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode > > callings;
	for (lcsm::Instruction &instant : m_instantsWiring)
	{
		value |= instant.value();
		callings.emplace(instant.caller());
	}
	for (lcsm::Instruction &instant : m_instantsTunnel)
	{
		value |= instant.value();
		callings.emplace(instant.caller());
	}

	// Go through all objects-neighbour and create a new events to all non callings.
	if (callings.count(m_wiring) == 0)
	{
		CreateInstr(this, m_wiring.get(), value, events);
	}
	for (lcsm::support::PointerView< lcsm::EvaluatorNode > &tunnel : m_tunnels)
	{
		if (callings.count(tunnel))
		{
			continue;
		}
		CreateInstr(this, tunnel.get(), value, events);
	}

	// Go through all objects-neighbour and create a new events to all callings, where instant::value() != value.
	for (lcsm::Instruction &instant : m_instantsWiring)
	{
		if (value != instant.value())
		{
			CreateInstr(this, instant.caller(), value, events);
		}
	}
	for (lcsm::Instruction &instant : m_instantsTunnel)
	{
		if (value != instant.value())
		{
			CreateInstr(this, instant.caller(), value, events);
		}
	}

	// Save last value.
	m_context->updateValues(now, { value });

	// Clear instants.
	m_instantsWiring.clear();
	m_instantsTunnel.clear();

	return events;
}

void lcsm::physical::Tunnel::connectWiring(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_wiring = node;
}

void lcsm::physical::Tunnel::connectTunnel(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_tunnels.push_back(node);
}
