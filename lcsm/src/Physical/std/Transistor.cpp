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
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Transistor::Transistor(lcsm::object_type_t objectType, lcsm::model::Transistor::Type type) :
	lcsm::EvaluatorNode(objectType), m_type(type)
{
}

const lcsm::DataBits &lcsm::physical::Transistor::read() const
{
	return m_context->getValue();
}

lcsm::Width lcsm::physical::Transistor::width() const
{
	return read().width();
}

bool lcsm::physical::Transistor::checkWidth(const DataBits &value) const
{
	return width() == value.width();
}

lcsm::NodeType lcsm::physical::Transistor::nodeType() const noexcept
{
	return lcsm::NodeType::Dynamic;
}

std::size_t lcsm::physical::Transistor::contextSize() const noexcept
{
	return 3;
}

void lcsm::physical::Transistor::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	if (context->size() != contextSize())
		throw std::logic_error("Transistor requires 3 elements in context.");
	m_context = context;
}

void lcsm::physical::Transistor::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Transistor::addInstant(const lcsm::Instruction &instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType instructionType = instruction.type();

	if (target != this)
		throw std::logic_error("Target is not this object.");

	if (instructionType != lcsm::InstructionType::WriteValue)
		throw std::logic_error("Bad instruction!");

	if (caller == m_base)
	{
		m_instantsBase.push_back(instruction);
		return;
	}

	if (caller == m_srca)
	{
		m_instantsSrcA.push_back(instruction);
		return;
	}

	if (caller == m_srcb)
	{
		m_instantsSrcB.push_back(instruction);
		return;
	}

	throw std::logic_error("Bad port!");
}

void lcsm::physical::Transistor::addInstant(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType instructionType = instruction.type();

	if (target != this)
		throw std::logic_error("Target is not this object.");

	if (instructionType != lcsm::InstructionType::WriteValue)
		throw std::logic_error("Bad instruction!");

	if (caller == m_base)
	{
		m_instantsBase.push_back(std::move(instruction));
		return;
	}

	if (caller == m_srca)
	{
		m_instantsSrcA.push_back(std::move(instruction));
		return;
	}

	if (caller == m_srcb)
	{
		m_instantsSrcB.push_back(std::move(instruction));
		return;
	}

	throw std::logic_error("Bad port!");
}

std::vector< lcsm::Event > lcsm::physical::Transistor::invokeInstants(const Timestamp &now)
{
	/* Values from wires. */
	lcsm::DataBits valueBase = m_context->getValue(0);
	lcsm::DataBits valueSrcA = m_context->getValue(1);
	lcsm::DataBits valueSrcB = m_context->getValue(2);

	/* If NOW is later... */
	const lcsm::Timestamp then = m_context->lastUpdate();
	if (now > then)
	{
		if (!m_instantsBase.empty())
		{
			const lcsm::Instruction instant = m_instantsBase.front();
			valueBase = instant.caller()->read();
			m_instantsBase.pop_front();
		}
		if (!m_instantsSrcA.empty())
		{
			const lcsm::Instruction instant = m_instantsSrcA.front();
			valueSrcA = instant.caller()->read();
			m_instantsSrcA.pop_front();
		}
		if (!m_instantsSrcB.empty())
		{
			const lcsm::Instruction instant = m_instantsSrcB.front();
			valueSrcB = instant.caller()->read();
			m_instantsSrcB.pop_front();
		}
	}

	/* Invoke all instructions. */
	for (lcsm::Instruction &instant : m_instantsBase)
		valueBase |= instant.caller()->read();
	for (lcsm::Instruction &instant : m_instantsSrcA)
		valueSrcA |= instant.caller()->read();
	for (lcsm::Instruction &instant : m_instantsSrcB)
		valueSrcB |= instant.caller()->read();

	/* Values must be 1 bit (aka width == Width::Bit1)! */
	if (valueBase.width() != lcsm::Width::Bit1 || valueSrcA != lcsm::Width::Bit1 || valueSrcB != lcsm::Width::Bit1)
		throw std::logic_error("Values in transistor must be 1bit!");

	/* Clear instants. */
	m_instantsBase.clear();
	m_instantsSrcA.clear();
	m_instantsSrcB.clear();

	/* Perform Verilog's transistor. */
	lcsm::DataBits newValueBase = valueBase;
	lcsm::DataBits newValueSrcA;
	lcsm::DataBits newValueSrcB;

	// TODO: Implement hardcoded situation.
	if (m_type == lcsm::model::Transistor::Type::N)
	{
	}
	else
	{
	}

	/* Update last value. */
	m_context->updateValues(now, { newValueBase, newValueSrcA, newValueSrcB });

	/* If new values equals to old and there is exisiting connection, then make events to write. */
	std::vector< lcsm::Event > events;

	if (m_srca && valueSrcA != newValueSrcA)
	{
		lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, m_srca.ptr());
		events.emplace_back(std::move(i));
	}

	if (m_srcb && valueSrcA != newValueSrcB)
	{
		lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, m_srcb.ptr());
		events.emplace_back(std::move(i));
	}

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
