#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Constant.h>
#include <lcsm/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>

lcsm::physical::Constant::Constant(const lcsm::DataBits &databits) : m_databits(databits) {}

lcsm::physical::Constant::Constant(lcsm::DataBits &&databits) : m_databits(std::move(databits)) {}

lcsm::NodeType lcsm::physical::Constant::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

const lcsm::DataBits &lcsm::physical::Constant::read() const
{
	return m_databits;
}

lcsm::Width lcsm::physical::Constant::width() const
{
	return read().width();
}

bool lcsm::physical::Constant::checkWidth(const lcsm::DataBits &value) const
{
	return width() == value.width();
}

void lcsm::physical::Constant::setContext(lcsm::support::PointerView< lcsm::Context > &context) noexcept
{
	m_context = context;
}

void lcsm::physical::Constant::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Constant::addInstant(const lcsm::Instruction &instruction)
{
	/* No instructions can be provided to Constant. */
	if (instruction.caller() == m_connect && instruction.type() == lcsm::InstructionType::WriteValue)
		return;
	throw std::logic_error("Attempt to instant instruction for Constant");
}

void lcsm::physical::Constant::addInstant(lcsm::Instruction &&instruction)
{
	/* No instructions can be provided to Constant. */
	if (instruction.caller() == m_connect && instruction.type() == lcsm::InstructionType::WriteValue)
		return;
	throw std::logic_error("Attempt to instant instruction for Constant");
}

std::vector< lcsm::Event > lcsm::physical::Constant::invokeInstants(const lcsm::Timestamp &now)
{
	/* Update context ONLY ONCE to be like a real constant element. */
	if (m_context->neverUpdate())
		m_context->updateValue(now, m_databits);

	/* Resulting events for future mini-steps. */
	std::vector< lcsm::Event > events;

	/* Target from this object. */
	const lcsm::support::PointerView< lcsm::EvaluatorNode > targetFrom = static_cast< lcsm::EvaluatorNode * >(this);
	lcsm::EvaluatorNode *c = static_cast< lcsm::EvaluatorNode * >(m_connect.ptr());

	/* Write value to Wire. */
	lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, c);
	lcsm::Event e = lcsm::CreateInstantEvent(std::move(i), targetFrom, m_connect);
	events.push_back(std::move(e));

	return events;
}

void lcsm::physical::Constant::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_connect = node;
}
