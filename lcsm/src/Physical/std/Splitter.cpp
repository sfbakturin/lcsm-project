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

lcsm::physical::Splitter::Splitter(lcsm::object_type_t objectType) :
	lcsm::EvaluatorNode(objectType), m_wasPollutedInput(false), m_wasPollutedOutput(false)
{
}

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

void lcsm::physical::Splitter::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

	// Target must be always this.
	if (target != this)
	{
		throw std::logic_error("Target is not this circuit!");
	}

	// Check if instruction supported.
	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		// If caller is one of outputs, then ignore this instruction.
		const std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > >::const_iterator foundOutput = std::find_if(
			m_outputs.begin(),
			m_outputs.end(),
			[caller](const lcsm::support::PointerView< lcsm::EvaluatorNode > &item) noexcept { return item == caller; });
		if (foundOutput != m_outputs.end())
		{
			return;
		}

		// If caller is input, instruction is valid.
		if (m_input == caller)
		{
			m_instants.push_back(std::move(instruction));
			return;
		}

		break;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		// If caller is one of outputs, then remember about it.
		const std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > >::const_iterator foundOutput = std::find_if(
			m_outputs.begin(),
			m_outputs.end(),
			[caller](const lcsm::support::PointerView< lcsm::EvaluatorNode > &item) noexcept { return item == caller; });
		if (foundOutput != m_outputs.end())
		{
			m_wasPollutedOutput = true;
			return;
		}

		// If caller is input, then remember about it.
		if (m_input == caller)
		{
			m_wasPollutedInput = true;
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

void lcsm::physical::Splitter::invoke(const lcsm::Timestamp &now, std::deque< lcsm::Event > &events)
{
	// Extract value from context.
	lcsm::DataBits value = m_context->getValue(0);
	const lcsm::Timestamp then = m_context->lastUpdate();

	// If NOW is later, then use value from instants.
	if (now > then && !m_instants.empty())
	{
		lcsm::Instruction &instruction = m_instants.front();
		value = std::move(instruction.value());
		m_instants.pop_front();
	}

	// Invoke all instructions.
	for (const lcsm::Instruction &instruction : m_instants)
	{
		value |= instruction.value();
	}

	// Clear instants.
	m_instants.clear();

	// Create WriteValue instructions to outs with specified indexes for subdatabits.
	for (std::size_t i = 0; i < m_indexes.size(); i++)
	{
		const std::pair< lcsm::width_t, lcsm::width_t > &index = m_indexes[i];
		const lcsm::width_t begin = index.first;
		const lcsm::width_t end = index.second;
		lcsm::Instruction I = lcsm::CreateWriteValueInstruction(this, m_outputs[i].get(), value.subdatabits(begin, end));
		lcsm::Event E{ std::move(I) };
		events.push_back(std::move(E));
	}

	// Save value to context.
	m_context->updateValues(now, { value });
}

void lcsm::physical::Splitter::connectInput(const lcsm::support::PointerView< lcsm::EvaluatorNode > &input) noexcept
{
	m_input = input;
}

void lcsm::physical::Splitter::connectOut(const lcsm::support::PointerView< lcsm::EvaluatorNode > &out,
										  const std::pair< lcsm::width_t, lcsm::width_t > &index)
{
	m_outputs.push_back(out);
	m_indexes.push_back(index);
}
