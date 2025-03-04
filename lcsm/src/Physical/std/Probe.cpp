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

void lcsm::physical::Probe::addInstant(const lcsm::Instruction &instruction)
{
	if (instruction.type() == lcsm::InstructionType::WriteValue && instruction.target() == this && instruction.caller() == m_wire)
		m_instants.push_back(instruction);
	else
		throw std::logic_error("Bad instant");
}

void lcsm::physical::Probe::addInstant(lcsm::Instruction &&instruction)
{
	if (instruction.type() == lcsm::InstructionType::WriteValue && instruction.target() == this && instruction.caller() == m_wire)
		m_instants.push_back(std::move(instruction));
	else
		throw std::logic_error("Bad instant");
}

std::vector< lcsm::Event > lcsm::physical::Probe::invokeInstants(const lcsm::Timestamp &now)
{
	/* Extract values from context. */
	lcsm::DataBits value = m_context->getValue(0);
	const lcsm::Timestamp &then = m_context->lastUpdate();
	const bool takeFirst = now > then;

	/* If NOW is later, then THEN, then we should take first value as not-dirty. */
	if (takeFirst && !m_instants.empty())
	{
		lcsm::Instruction instant = m_instants.front();
		m_instants.pop_front();
		value = instant.value();
	}

	/* Invoke all instructions. */
	for (const lcsm::Instruction &instant : m_instants)
		value |= instant.value();

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
