#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Constant.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>
#include <stdexcept>
#include <utility>

lcsm::physical::Constant::Constant(lcsm::object_type_t objectType, const lcsm::DataBits &databits) noexcept :
	lcsm::EvaluatorNode(objectType), m_wasPolluted(false), m_databits(databits)
{
}

lcsm::physical::Constant::Constant(lcsm::object_type_t objectType, lcsm::DataBits &&databits) noexcept :
	lcsm::EvaluatorNode(objectType), m_wasPolluted(false), m_databits(std::move(databits))
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

void lcsm::physical::Constant::add(lcsm::Instruction &&instruction)
{
	const lcsm::instruction_t type = instruction.type();

	// Check for supported instruction.
	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		return;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		m_wasPolluted = true;
		return;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad instruction type!");
}

void lcsm::physical::Constant::invoke(const lcsm::Timestamp &now, std::deque< lcsm::Event > &events)
{
	// Update context ONLY ONCE to be like a real constant element.
	if (m_context->neverUpdate())
	{
		m_context->updateValues(now, { m_databits });
	}

	// If it was polluted, then generate simulator's event.
	// Otherwise, act normally.
	if (m_wasPolluted)
	{
		// Create new simulator instruction.
		events.emplace_back(lcsm::CreatePolluteCircuitSimulatorInstruction(this));

		// Reset.
		m_wasPolluted = false;

		// Return.
		return;
	}

	// Write value to Wire.
	events.push_back(lcsm::CreateWriteValueInstruction(this, m_connect.get(), m_databits));
}

void lcsm::physical::Constant::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_connect = node;
}
