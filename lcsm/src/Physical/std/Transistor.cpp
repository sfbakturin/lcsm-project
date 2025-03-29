#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/std/Transistor.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Transistor::Transistor(lcsm::object_type_t objectType, lcsm::model::Transistor::Type type) :
	lcsm::EvaluatorNode(objectType), m_type(type), m_wasPollutedFromSrcA(false), m_wasPollutedFromSrcB(false)
{
}

lcsm::NodeType lcsm::physical::Transistor::nodeType() const noexcept
{
	return lcsm::NodeType::Dynamic;
}

std::size_t lcsm::physical::Transistor::contextSize() const noexcept
{
	return 3;
}

std::size_t lcsm::physical::Transistor::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Transistor::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
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

void lcsm::physical::Transistor::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Transistor::verifyContext()
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

	const lcsm::DataBits &base = m_context->getValue(0);
	const lcsm::DataBits &srcA = m_context->getValue(1);
	const lcsm::DataBits &srcB = m_context->getValue(2);
	const lcsm::Width desired = lcsm::Width::Bit1;
	if (base.width() != desired || srcA.width() != desired || srcB.width() != desired)
	{
		resetContext();
		throw std::logic_error("Bad value width!");
	}
}

void lcsm::physical::Transistor::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

	if (target != this)
	{
		throw std::logic_error("Target is not this object.");
	}

	switch (type)
	{
	case lcsm::InstructionType::WriteValue:
	{
		if (m_base == caller)
		{
			m_instantsBase.push_back(std::move(instruction));
			return;
		}

		if (m_srca == caller)
		{
			m_instantsSrcA.push_back(std::move(instruction));
			return;
		}

		if (m_srcb == caller)
		{
			m_instantsSrcB.push_back(std::move(instruction));
			return;
		}

		break;
	}
	case lcsm::InstructionType::PolluteValue:
	{
		if (m_base == caller)
		{
			return;
		}

		if (m_srca == caller)
		{
			m_wasPollutedFromSrcA = true;
			return;
		}

		if (m_srcb == caller)
		{
			m_wasPollutedFromSrcB = true;
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

std::vector< lcsm::Event > lcsm::physical::Transistor::invoke(const Timestamp &now)
{
	// Constants.
	static const lcsm::DataBits NO_VALUE(lcsm::Width::Bit1, lcsm::verilog::Strength::HighImpedance, lcsm::verilog::Bit::Undefined);

	// Values from wires.
	lcsm::DataBits valueBase = m_context->getValue(0);
	lcsm::DataBits valueSrcA = m_context->getValue(1);
	lcsm::DataBits valueSrcB = m_context->getValue(2);

	// Determine, if there was signal from sources.
	const bool wasSignalFromSrcA = !m_instantsSrcA.empty();
	const bool wasSignalFromSrcB = !m_instantsSrcB.empty();

	// If NOW is later...
	const lcsm::Timestamp thenBase = m_context->lastUpdate(0);
	const lcsm::Timestamp thenSrcA = m_context->lastUpdate(1);
	const lcsm::Timestamp thenSrcB = m_context->lastUpdate(2);

	if (now > thenBase && !m_instantsBase.empty())
	{
		lcsm::Instruction &instruction = m_instantsBase.front();
		valueBase = instruction.value();
		m_instantsBase.pop_front();
	}

	if (now > thenSrcA && !m_instantsSrcA.empty())
	{
		lcsm::Instruction &instruction = m_instantsSrcA.front();
		valueSrcA = instruction.value();
		m_instantsSrcA.pop_front();
	}

	if (now > thenSrcB && !m_instantsSrcB.empty())
	{
		lcsm::Instruction &instruction = m_instantsSrcB.front();
		valueSrcB = instruction.value();
		m_instantsSrcB.pop_front();
	}

	// Invoke all instructions.
	for (const lcsm::Instruction &instruction : m_instantsBase)
	{
		valueBase |= instruction.value();
	}

	for (const lcsm::Instruction &instruction : m_instantsSrcA)
	{
		valueSrcA |= instruction.value();
	}

	for (const lcsm::Instruction &instruction : m_instantsSrcB)
	{
		valueSrcB |= instruction.value();
	}

	// Values must be 1 bit (aka width == Width::Bit1)!
	if (valueBase.width() != lcsm::Width::Bit1 || valueSrcA.width() != lcsm::Width::Bit1 || valueSrcB.width() != lcsm::Width::Bit1)
	{
		throw std::logic_error("Values in transistor must be 1bit!");
	}

	// Clear instants.
	m_instantsBase.clear();
	m_instantsSrcA.clear();
	m_instantsSrcB.clear();

	// Perform Verilog's transistor.
	lcsm::DataBits newValueBase = valueBase;
	lcsm::DataBits newValueSrcA = valueSrcA;
	lcsm::DataBits newValueSrcB = valueSrcB;

	// If new values equals to old and there is existing connection, then make events to write.
	std::vector< lcsm::Event > events;

	// Logisim Evolution extended.
	const lcsm::verilog::Bit bitOpen =
		(m_type == lcsm::model::Transistor::Type::N ? lcsm::verilog::Bit::True : lcsm::verilog::Bit::False);

	// Check, If base is opened.
	// Otherwise, no actions provided.
	if (newValueBase.bit(0) == bitOpen && newValueBase.strength(0) != lcsm::verilog::Strength::HighImpedance)
	{
		// If there was pollution (1).
		if (m_wasPollutedFromSrcA && !m_wasPollutedFromSrcB)
		{
			events.emplace_back(lcsm::CreatePolluteValueInstruction(this, m_srcb.get()));
			events.emplace_back(lcsm::CreatePolluteValueInstruction(this, m_base.ptr()));
		}

		// If there was pollution (2).
		if (!m_wasPollutedFromSrcA && m_wasPollutedFromSrcB)
		{
			events.emplace_back(lcsm::CreatePolluteValueInstruction(this, m_srca.get()));
			events.emplace_back(lcsm::CreatePolluteValueInstruction(this, m_base.ptr()));
		}

		// If only one value is provided (1).
		if (wasSignalFromSrcA && !wasSignalFromSrcB)
		{
			newValueSrcA = valueSrcA;
			newValueSrcB = valueSrcA;
			events.emplace_back(lcsm::CreateWriteValueInstruction(this, m_srcb.ptr(), std::move(valueSrcA)));
		}	 // If only one value is provided (2).
		else if (!wasSignalFromSrcA && wasSignalFromSrcB)
		{
			newValueSrcB = valueSrcB;
			newValueSrcA = valueSrcB;
			events.emplace_back(lcsm::CreateWriteValueInstruction(this, m_srca.ptr(), std::move(valueSrcB)));
		}
		else if (wasSignalFromSrcA && wasSignalFromSrcB)
		{
			// If two values is provided.
			const lcsm::DataBits newValueResult = newValueSrcA | newValueSrcB;
			if (newValueResult != newValueSrcA)
			{
				newValueSrcA = newValueResult;
				events.emplace_back(lcsm::CreateWriteValueInstruction(this, m_srca.ptr(), newValueSrcA));
			}

			if (newValueResult != newValueSrcB)
			{
				newValueSrcB = newValueResult;
				events.emplace_back(lcsm::CreateWriteValueInstruction(this, m_srcb.ptr(), newValueSrcB));
			}
		}
		else
		{
			// Pollute the circuit in sources directions.
			events.emplace_back(lcsm::CreatePolluteValueInstruction(this, m_srca.get()));
			events.emplace_back(lcsm::CreatePolluteValueInstruction(this, m_srcb.get()));
		}
	}

	m_context->setPolluted(m_wasPollutedFromSrcA || m_wasPollutedFromSrcB);

	m_context->updateValue(0, std::move(newValueBase), now);
	m_context->updateValue(1, std::move(newValueSrcA), now);
	m_context->updateValue(2, std::move(newValueSrcB), now);

	m_wasPollutedFromSrcA = false;
	m_wasPollutedFromSrcB = false;

	return events;
}

void lcsm::physical::Transistor::connectBase(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_base = node;
}

void lcsm::physical::Transistor::connectSrcA(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_srca = node;
}

void lcsm::physical::Transistor::connectSrcB(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_srcb = node;
}
