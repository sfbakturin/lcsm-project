#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Constant.h>
#include <lcsm/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>

lcsm::physical::Constant::Constant(lcsm::object_type_t objectType, const lcsm::DataBits &databits) :
	lcsm::EvaluatorNode(objectType), m_wasPolluteInstant(false), m_databits(databits)
{
}

lcsm::physical::Constant::Constant(lcsm::object_type_t objectType, lcsm::DataBits &&databits) :
	lcsm::EvaluatorNode(objectType), m_wasPolluteInstant(false), m_databits(std::move(databits))
{
}

lcsm::NodeType lcsm::physical::Constant::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

std::size_t lcsm::physical::Constant::contextSize() const noexcept
{
	return 1;
}

std::size_t lcsm::physical::Constant::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Constant::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
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

void lcsm::physical::Constant::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Constant::verifyContext()
{
	// Check global sizes.
	if (m_context->size() != contextSize() || m_context->privateSize() != privateContextSize())
	{
		resetContext();
		throw std::logic_error("Bad context size!");
	}

	// Check value width, only when there was an update at once.
	if (m_context->neverUpdate())
	{
		return;
	}

	// Check value.
	const lcsm::DataBits &value = m_context->getValue();
	if (value.width() != m_databits.width())
	{
		resetContext();
		throw std::logic_error("Bad value width!");
	}
}

void lcsm::physical::Constant::addInstant(const lcsm::Instruction &instruction)
{
	const lcsm::InstructionType type = instruction.type();

	// Check for supported instruction. If its WriteValue - ignore, if its PolluteValue, then remember to answer on
	// pollution.
	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		return;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		m_wasPolluteInstant = true;
		return;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad instruction type!");
}

void lcsm::physical::Constant::addInstant(lcsm::Instruction &&instruction)
{
	const lcsm::InstructionType type = instruction.type();

	// Check for supported instruction. If its WriteValue - ignore, if its PolluteValue, then remember to answer on
	// pollution.
	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		return;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		m_wasPolluteInstant = true;
		return;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad instruction type!");
}

std::vector< lcsm::Event > lcsm::physical::Constant::invokeInstants(const lcsm::Timestamp &now)
{
	// Update context ONLY ONCE to be like a real constant element.
	if (m_context->neverUpdate())
	{
		m_context->updateValues(now, { m_databits });
	}

	// Resulting events for future mini-steps.
	std::vector< lcsm::Event > events;

	// There might be pollution.
	const lcsm::Timestamp diff(0, static_cast< lcsm::timescale_t >(m_wasPolluteInstant));
	if (m_wasPolluteInstant)
	{
		int x = 1;
	}
	m_wasPolluteInstant = false;

	// Write value to Wire.
	lcsm::Instruction I = lcsm::CreateWriteValueInstruction(this, m_connect.get(), m_databits);
	lcsm::Event E(std::move(I), diff);
	events.push_back(std::move(E));

	return events;
}

void lcsm::physical::Constant::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_connect = node;
}
