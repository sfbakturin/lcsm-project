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
	return 1;
}

void lcsm::physical::Digit::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	if (contextSize() != context->size())
		throw std::logic_error("Bad context size!");
	m_context = context;
}

void lcsm::physical::Digit::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Digit::addInstant(const lcsm::Instruction &instruction)
{
	const lcsm::InstructionType type = instruction.type();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::EvaluatorNode *caller = instruction.caller();
	if (target != this)
		throw std::logic_error("Target is not this element");
	if (type == lcsm::InstructionType::WriteValue && caller == m_data)
		m_instantsData.push_back(instruction);
	else if (type == lcsm::InstructionType::WriteValue && caller == m_decimalPoint && m_hasDecimalPoint)
		m_instantsDecimalPoint.push_back(instruction);
	else
		throw std::logic_error("Bad instant!");
}

void lcsm::physical::Digit::addInstant(lcsm::Instruction &&instruction)
{
	const lcsm::InstructionType type = instruction.type();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::EvaluatorNode *caller = instruction.caller();
	if (target != this)
		throw std::logic_error("Target is not this element");
	if (type == lcsm::InstructionType::WriteValue && caller == m_data)
		m_instantsData.push_back(std::move(instruction));
	else if (type == lcsm::InstructionType::WriteValue && caller == m_decimalPoint && m_hasDecimalPoint)
		m_instantsDecimalPoint.push_back(std::move(instruction));
	else
		throw std::logic_error("Bad instant!");
}

std::vector< lcsm::Event > lcsm::physical::Digit::invokeInstants(const lcsm::Timestamp &now)
{
	/* Extract values from context. */
	lcsm::DataBits valueData = m_context->getValue(0);
	lcsm::DataBits valueDecimalPoint = m_context->getValue(1);
	const lcsm::Timestamp &then = m_context->lastUpdate();
	const bool takeFirst = now > then;

	/* If NOW is later, then THEN, then we should take first value as not-dirty. */
	if (takeFirst)
	{
		if (!m_instantsData.empty())
		{
			lcsm::Instruction instant = m_instantsData.front();
			m_instantsData.pop_front();
			valueData = instant.value();
		}
		if (!m_instantsDecimalPoint.empty())
		{
			lcsm::Instruction instant = m_instantsDecimalPoint.front();
			m_instantsDecimalPoint.pop_front();
			valueDecimalPoint = instant.value();
		}
	}

	/* Invoke all instructions. */
	for (const lcsm::Instruction &instant : m_instantsData)
		valueData |= instant.value();
	for (const lcsm::Instruction &instant : m_instantsDecimalPoint)
		valueDecimalPoint |= instant.value();

	/* Clean instants. */
	m_instantsData.clear();
	m_instantsDecimalPoint.clear();

	/* Update context. */
	/* MAYBE, valueData must be exactly 4 bits and valueDecimalPoint must be exactly 1 bit... */
	m_context->updateValues(now, { valueData, valueDecimalPoint });

	/* No events from Digit. */
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
