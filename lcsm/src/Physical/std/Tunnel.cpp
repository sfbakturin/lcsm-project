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

#include <algorithm>
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

void lcsm::physical::Tunnel::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

	// Check, if target is this circuit element.
	if (target != this)
	{
		throw std::logic_error("Bad this target element!");
	}

	// Find caller in tunnels.
	const std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > >::const_iterator found = std::find_if(
		m_tunnels.begin(),
		m_tunnels.end(),
		[caller](const lcsm::support::PointerView< lcsm::EvaluatorNode > &c) { return c == caller; });

	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		if (m_wiring == caller)
		{
			m_writeValueWiring.push_back(std::move(instruction));
			return;
		}
		else if (found != m_tunnels.end())
		{
			m_writeValueTunnel.push_back(std::move(instruction));
			return;
		}
		break;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		if (m_wiring == caller)
		{
			m_polluteValueWiring.push_back(std::move(instruction));
			return;
		}
		else if (found != m_tunnels.end())
		{
			m_polluteValueTunnel.push_back(std::move(instruction));
			return;
		}
		break;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad instruction!");
}

std::vector< lcsm::Event > lcsm::physical::Tunnel::invoke(const lcsm::Timestamp &now)
{
	// Created events.
	std::vector< lcsm::Event > events;

	// Extract contexted value.
	lcsm::DataBits value = m_context->getValue();
	const lcsm::Timestamp &then = m_context->lastUpdate();
	const bool takeFirst = now > then;
	bool popedWiring = false;
	bool popedTunnel = false;

	// If NOW is later, then THEN, then we should take first value as not-dirty.
	if (takeFirst)
	{
		const bool hasWiring = !m_writeValueWiring.empty();
		const bool hasTunnel = !m_writeValueTunnel.empty();
		if (hasWiring && hasTunnel)
		{
			lcsm::Instruction &instantWiring = m_writeValueWiring.front();
			value = std::move(instantWiring.value());
			lcsm::Instruction &instantTunnel = m_writeValueTunnel.front();
			value |= instantTunnel.value();
			m_writeValueWiring.pop_front();
			m_writeValueTunnel.pop_front();
			popedTunnel = true;
			popedWiring = true;
		}
		else if (hasWiring && !hasTunnel)
		{
			lcsm::Instruction &instantWiring = m_writeValueWiring.front();
			value = std::move(instantWiring.value());
			m_writeValueWiring.pop_front();
			popedWiring = true;
		}
		else if (!hasWiring && hasTunnel)
		{
			lcsm::Instruction &instantTunnel = m_writeValueTunnel.front();
			value = std::move(instantTunnel.value());
			m_writeValueTunnel.pop_front();
			popedTunnel = true;
		}
	}

	// Implementation of pollution.
	if (!m_polluteValueTunnel.empty() || !m_polluteValueWiring.empty())
	{
		// Get all polluters.
		std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode > > polluters;
		for (lcsm::Instruction &instruction : m_polluteValueWiring)
		{
			polluters.emplace(instruction.caller());
		}
		for (lcsm::Instruction &instruction : m_polluteValueTunnel)
		{
			polluters.emplace(instruction.caller());
		}

		// Go through all objects-neighbour and create a new events to all non callings.
		if (polluters.find(m_wiring) == polluters.end())
		{
			events.emplace_back(lcsm::CreatePolluteValueInstruction(this, m_wiring.get()));
		}

		for (lcsm::support::PointerView< lcsm::EvaluatorNode > &tunnel : m_tunnels)
		{
			if (polluters.find(tunnel) == polluters.end())
			{
				events.emplace_back(lcsm::CreatePolluteValueInstruction(this, tunnel.get()));
			}
		}

		// Set context to polluted.
		m_context->setPolluted(true);
	}

	// Implementation of writing value.
	if (!m_writeValueTunnel.empty() || !m_writeValueWiring.empty() || popedTunnel || popedWiring)
	{
		// Invoke all instructions.
		std::unordered_set< lcsm::support::PointerView< lcsm::EvaluatorNode > > callings;
		for (lcsm::Instruction &instruction : m_writeValueWiring)
		{
			value |= instruction.value();
			callings.emplace(instruction.caller());
		}
		for (lcsm::Instruction &instruction : m_writeValueTunnel)
		{
			value |= instruction.value();
			callings.emplace(instruction.caller());
		}

		// Go through all objects-neighbour and create a new events to all non callings.
		if (callings.find(m_wiring) == callings.end())
		{
			events.emplace_back(lcsm::CreateWriteValueInstruction(this, m_wiring.get(), value));
		}

		for (lcsm::support::PointerView< lcsm::EvaluatorNode > &tunnel : m_tunnels)
		{
			if (callings.find(tunnel) == callings.end())
			{
				events.emplace_back(lcsm::CreateWriteValueInstruction(this, tunnel.get(), value));
			}
		}

		// Go through all objects-neighbour and create a new events to all callings, where instant::value() != value.
		for (lcsm::Instruction &instruction : m_writeValueWiring)
		{
			if (value != instruction.value())
			{
				events.emplace_back(lcsm::CreateWriteValueInstruction(this, instruction.caller(), value));
			}
		}

		for (lcsm::Instruction &instruction : m_writeValueTunnel)
		{
			if (value != instruction.value())
			{
				events.emplace_back(lcsm::CreateWriteValueInstruction(this, instruction.caller(), value));
			}
		}

		// Set context as not polluted.
		m_context->setPolluted(false);
	}

	// Save last value.
	m_context->updateValues(now, { value });

	// Clear instants.
	m_writeValueWiring.clear();
	m_writeValueTunnel.clear();
	m_polluteValueTunnel.clear();
	m_polluteValueWiring.clear();

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
