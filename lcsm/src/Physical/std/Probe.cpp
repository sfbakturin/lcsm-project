#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Probe.h>
#include <lcsm/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Probe::Probe(lcsm::object_type_t objectType) : lcsm::EvaluatorNode(objectType) {}

lcsm::NodeType lcsm::physical::Probe::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

std::size_t lcsm::physical::Probe::contextSize() const noexcept
{
	return 1;
}

std::size_t lcsm::physical::Probe::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Probe::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
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

void lcsm::physical::Probe::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Probe::verifyContext()
{
	// Check global sizes.
	if (m_context->size() != contextSize() || m_context->privateSize() != privateContextSize())
	{
		resetContext();
		throw std::logic_error("Bad context size!");
	}
}

void lcsm::physical::Probe::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

	if (target != this)
	{
		throw std::logic_error("Bad target in instruction!");
	}

	if (m_wire != caller)
	{
		throw std::logic_error("Bad caller in instruction!");
	}

	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		m_instants.push_back(std::move(instruction));
		return;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		return;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad instruction!");
}

std::vector< lcsm::Event > lcsm::physical::Probe::invoke(const lcsm::Timestamp &now)
{
	/* Extract values from context. */
	lcsm::DataBits value = m_context->getValue(0);
	const lcsm::Timestamp &then = m_context->lastUpdate();
	const bool takeFirst = now > then;

	/* If NOW is later, then THEN, then we should take first value as not-dirty. */
	if (takeFirst && !m_instants.empty())
	{
		lcsm::Instruction &instruction = m_instants.front();
		value = std::move(instruction.value());
		m_instants.pop_front();
	}

	/* Invoke all instructions. */
	for (const lcsm::Instruction &instruction : m_instants)
	{
		value |= instruction.value();
	}

	/* Clean instants. */
	m_instants.clear();

	/* Update context. */
	m_context->updateValues(now, { value });

	/* No events from Probe. */
	return {};
}

void lcsm::physical::Probe::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &wire)
{
	m_wire = wire;
}
