#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Clock.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Bit.h>

#include <stdexcept>
#include <vector>

lcsm::physical::Clock::Clock(lcsm::ObjectType objectType, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset) :
	lcsm::EvaluatorNode(objectType), m_highDuration(highDuration), m_lowDuration(lowDuration), m_phaseOffset(phaseOffset)
{
}

lcsm::NodeType lcsm::physical::Clock::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

std::size_t lcsm::physical::Clock::contextSize() const noexcept
{
	return 1;
}

const lcsm::DataBits &lcsm::physical::Clock::read() const
{
	return m_context->getValue(0);
}

lcsm::Width lcsm::physical::Clock::width() const
{
	return read().width();
}

bool lcsm::physical::Clock::checkWidth(const lcsm::DataBits &value) const
{
	return width() == value.width();
}

void lcsm::physical::Clock::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	if (context->size() != contextSize())
		throw std::logic_error("Bad context size!");
	m_context = context;
}

void lcsm::physical::Clock::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Clock::addInstant(const lcsm::Instruction &instruction)
{
	if (instruction.caller() == m_connect && instruction.type() == lcsm::InstructionType::WriteValue)
		return;
	throw std::logic_error("Bad instant!");
}

void lcsm::physical::Clock::addInstant(lcsm::Instruction &&instruction)
{
	if (instruction.caller() == m_connect && instruction.type() == lcsm::InstructionType::WriteValue)
		return;
	throw std::logic_error("Bad instant!");
}

std::vector< lcsm::Event > lcsm::physical::Clock::invokeInstants(const lcsm::Timestamp &now)
{
	/* Resulting events at end of invoking. */
	std::vector< lcsm::Event > events;

	/* If there is no updates, then put 1bit-Strong-False to context. Otherwise, read from context. */
	lcsm::DataBits value;
	if (m_context->neverUpdate())
		value = { lcsm::Width::Bit1, lcsm::verilog::Bit::False };
	else
		value = m_context->getValue();

	// TODO: Implement somehow counter.

	/* Write value to Wire. */
	lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, m_connect.ptr());
	events.emplace_back(std::move(i));

	return events;
}

void lcsm::physical::Clock::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_connect = node;
}
