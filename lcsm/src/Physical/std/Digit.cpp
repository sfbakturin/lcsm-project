#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Digit.h>
#include <lcsm/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Digit::Digit(lcsm::object_type_t objectType, bool hasDecimalPoint) :
	lcsm::EvaluatorNode(objectType), m_hasDecimalPoint(hasDecimalPoint)
{
}

lcsm::NodeType lcsm::physical::Digit::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

std::size_t lcsm::physical::Digit::contextSize() const noexcept
{
	return 2;
}

std::size_t lcsm::physical::Digit::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Digit::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
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

void lcsm::physical::Digit::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Digit::verifyContext()
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
	const lcsm::DataBits &data = m_context->getValue(0);
	const lcsm::DataBits &decimalPoint = m_context->getValue(1);
	if ((data.width() != lcsm::Width::Bit4) || (m_hasDecimalPoint && decimalPoint.width() != lcsm::Width::Bit1))
	{
		resetContext();
		throw std::logic_error("Bad value width!");
	}
}

void lcsm::physical::Digit::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

	if (target != this)
	{
		throw std::logic_error("Target is not this element");
	}

	// Check if instruction is supported.
	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		if (m_data == caller)
		{
			m_instantsData.push_back(std::move(instruction));
			return;
		}
		else if (m_hasDecimalPoint && m_decimalPoint == caller)
		{
			m_instantsDecimalPoint.push_back(std::move(instruction));
			return;
		}
		break;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		if (m_data == caller)
		{
			return;
		}
		else if (m_hasDecimalPoint && m_decimalPoint == caller)
		{
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

std::vector< lcsm::Event > lcsm::physical::Digit::invoke(const lcsm::Timestamp &now)
{
	// Extract values from context.
	lcsm::DataBits valueData = m_context->getValue(0);
	lcsm::DataBits valueDecimalPoint = m_context->getValue(1);
	const lcsm::Timestamp thenData = m_context->lastUpdate(0);
	const lcsm::Timestamp thenDecimalPoint = m_context->lastUpdate(1);

	// If NOW is later, then THEN, then we should take first value as not-dirty.
	if (now > thenData && !m_instantsData.empty())
	{
		lcsm::Instruction &instruction = m_instantsData.front();
		valueData = std::move(instruction.value());
		m_instantsData.pop_front();
	}

	if (now > thenDecimalPoint && !m_instantsDecimalPoint.empty())
	{
		lcsm::Instruction &instruction = m_instantsDecimalPoint.front();
		valueDecimalPoint = std::move(instruction.value());
		m_instantsDecimalPoint.pop_front();
	}

	// Invoke all instructions.
	for (const lcsm::Instruction &instruction : m_instantsData)
	{
		valueData |= instruction.value();
	}

	for (const lcsm::Instruction &instruction : m_instantsDecimalPoint)
	{
		valueDecimalPoint |= instruction.value();
	}

	// Clean instants.
	m_instantsData.clear();
	m_instantsDecimalPoint.clear();

	// Update context.
	m_context->updateValues(now, { valueData, valueDecimalPoint });
	verifyContext();

	// No events from Digit.
	return {};
}

void lcsm::physical::Digit::connectToData(const lcsm::support::PointerView< lcsm::EvaluatorNode > &wire)
{
	m_data = wire;
}

void lcsm::physical::Digit::connectToDecimalPoint(const lcsm::support::PointerView< lcsm::EvaluatorNode > &wire)
{
	if (!m_hasDecimalPoint)
		throw std::logic_error("Can't connect to decimal point!");
	m_decimalPoint = wire;
}
