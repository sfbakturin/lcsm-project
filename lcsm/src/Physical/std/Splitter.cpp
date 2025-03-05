#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Splitter.h>
#include <lcsm/Support/PointerView.hpp>

#include <algorithm>
#include <cstddef>
#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Splitter::Splitter(lcsm::object_type_t objectType) : lcsm::EvaluatorNode(objectType) {}

lcsm::NodeType lcsm::physical::Splitter::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

std::size_t lcsm::physical::Splitter::contextSize() const noexcept
{
	return 1;
}

std::size_t lcsm::physical::Splitter::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Splitter::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
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

void lcsm::physical::Splitter::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Splitter::verifyContext()
{
	// Check global sizes.
	if (m_context->size() != contextSize() || m_context->privateSize() != privateContextSize())
	{
		resetContext();
		throw std::logic_error("Bad context size!");
	}
}

void lcsm::physical::Splitter::addInstant(const lcsm::Instruction &instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType type = instruction.type();

	// Target must be always this.
	if (target != this)
	{
		throw std::logic_error("Target is not this circuit!");
	}

	// Only WriteValue is available for now.
	if (type != lcsm::InstructionType::WriteValue)
	{
		throw std::logic_error("Bad instruction!");
	}

	// If caller is one of outputs, then ignore this instruction.
	const std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > >::const_iterator foundOutput = std::find_if(
		m_outputs.begin(),
		m_outputs.end(),
		[caller](const lcsm::support::PointerView< lcsm::EvaluatorNode > &item) { return item == caller; });
	if (foundOutput != m_outputs.end())
	{
		return;
	}

	// If caller is not input, then this instruction is invalid.
	if (m_input != caller)
	{
		throw std::logic_error("Invalid instruction!");
	}

	// Otherwise, add this instruction to instants.
	m_instants.push_back(instruction);
}

void lcsm::physical::Splitter::addInstant(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType type = instruction.type();

	// Target must be always this.
	if (target != this)
	{
		throw std::logic_error("Target is not this circuit!");
	}

	// Only WriteValue is available for now.
	if (type != lcsm::InstructionType::WriteValue)
	{
		throw std::logic_error("Bad instruction!");
	}

	// If caller is one of outputs, then ignore this instruction.
	const std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > >::const_iterator foundOutput = std::find_if(
		m_outputs.begin(),
		m_outputs.end(),
		[caller](const lcsm::support::PointerView< lcsm::EvaluatorNode > &item) { return item == caller; });
	if (foundOutput != m_outputs.end())
	{
		return;
	}

	// If caller is not input, then this instruction is invalid.
	if (m_input != caller)
	{
		throw std::logic_error("Invalid instruction!");
	}

	// Otherwise, add this instruction to instants.
	m_instants.push_back(std::move(instruction));
}

std::vector< lcsm::Event > lcsm::physical::Splitter::invokeInstants(const lcsm::Timestamp &now)
{
	// Extract value from context.
	lcsm::DataBits value = m_context->getValue(0);
	const lcsm::Timestamp then = m_context->lastUpdate();

	// If NOW is later, then use value from instants.
	if (now > then && !m_instants.empty())
	{
		const lcsm::Instruction instant = m_instants.front();
		m_instants.pop_front();
		value = instant.value();
	}

	// Invoke all instructions.
	for (const lcsm::Instruction &instant : m_instants)
	{
		value |= instant.value();
	}

	// Clear instants.
	m_instants.clear();

	// Create WriteValue instructions to outs with specified indexes for subdatabits.
	std::vector< lcsm::Event > events;
	const lcsm::InstructionType type = lcsm::InstructionType::WriteValue;
	for (std::size_t i = 0; i < m_indexes.size(); i++)
	{
		const std::pair< lcsm::width_t, lcsm::width_t > &index = m_indexes[i];
		const lcsm::width_t begin = index.first;
		const lcsm::width_t end = index.second;
		lcsm::Instruction instruction{ type, this, m_outputs[i].get(), value.subdatabits(begin, end) };
		events.emplace_back(std::move(instruction));
	}

	// Save value to context.
	m_context->updateValues(now, { value });

	return events;
}

void lcsm::physical::Splitter::connectInput(const lcsm::support::PointerView< lcsm::EvaluatorNode > &input)
{
	m_input = input;
}

void lcsm::physical::Splitter::connectOut(const lcsm::support::PointerView< lcsm::EvaluatorNode > &out,
										  const std::pair< lcsm::width_t, lcsm::width_t > &index)
{
	m_outputs.push_back(out);
	m_indexes.push_back(index);
}
