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

#define UNUSED(X) ((void)(X))

lcsm::physical::Clock::Clock(lcsm::object_type_t objectType, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset) :
	lcsm::EvaluatorNode(objectType), m_highDuration(highDuration), m_lowDuration(lowDuration),
	m_phaseOffset(phaseOffset), m_counterFalse(m_lowDuration), m_counterTrue(m_highDuration), m_counter(true)
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

std::size_t lcsm::physical::Clock::privateContextSize() const noexcept
{
	return 3;
}

void lcsm::physical::Clock::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	if (context->size() != contextSize() || context->privateSize() != privateContextSize())
		throw std::logic_error("Bad context size!");

	// If already contexted, reset old, set new context and extract counters.
	// Otherwise, set context without extracting counters.
	if (m_context)
	{
		resetContext();
		m_context = context;
		m_counterFalse = m_context->privateContext().asInt(0);
		m_counterTrue = m_context->privateContext().asInt(1);
		m_counter = m_context->privateContext().asBool(2);
	}
	else
	{
		m_context = context;
	}
}

void lcsm::physical::Clock::resetContext() noexcept
{
	// Save counters.
	m_context->privateContext().putInt(0, m_counterFalse);
	m_context->privateContext().putInt(1, m_counterTrue);
	m_context->privateContext().putBool(2, m_counter);
	m_context.reset();
}

void lcsm::physical::Clock::addInstant(const lcsm::Instruction &instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType type = instruction.type();

	// Check if target is this circuit.
	if (target != this)
	{
		throw std::logic_error("Bad target in instruction!");
	}

	// Check if caller is known port.
	if (m_connect != caller)
	{
		throw std::logic_error("Unknown caller!");
	}

	// Check if instruction is supported one.
	if (type != lcsm::InstructionType::WriteValue)
	{
		throw std::logic_error("Bad instruction type!");
	}

	// Otherwise, ignore this instruction (no need for Clock element).
}

void lcsm::physical::Clock::addInstant(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType type = instruction.type();

	// Check if target is this circuit.
	if (target != this)
	{
		throw std::logic_error("Bad target in instruction!");
	}

	// Check if caller is known port.
	if (m_connect != caller)
	{
		throw std::logic_error("Unknown caller!");
	}

	// Check if instruction is supported one.
	if (type != lcsm::InstructionType::WriteValue)
	{
		throw std::logic_error("Bad instruction type!");
	}

	// Otherwise, ignore this instruction (no need for Clock element).
}

std::vector< lcsm::Event > lcsm::physical::Clock::invokeInstants(const lcsm::Timestamp &now)
{
	// Generated events.
	std::vector< lcsm::Event > events;

	// If there is no updates, then put 1bit-Strong-False to context. Otherwise, read from context.
	lcsm::DataBits value;
	if (m_context->neverUpdate())
	{
		value = { lcsm::Width::Bit1, lcsm::verilog::Bit::True };
	}
	else
	{
		value = m_context->getValue();
	}

	UNUSED(m_phaseOffset);

	// Check counter of value's bit (0 - False, 1 - True), if equals zero, then change value and reset counter,
	// otherwise, decrement and continue.
	if (!m_counter)
	{
		if (m_counterFalse == 0)
		{
			value.setBit(0, lcsm::verilog::Bit::True);
			m_counterFalse = m_lowDuration;
			m_counter = true;
		}
		else
		{
			m_counterFalse--;
		}
	}
	else
	{
		if (m_counterTrue == 0)
		{
			value.setBit(0, lcsm::verilog::Bit::False);
			m_counterTrue = m_highDuration;
			m_counter = false;
		}
		else
		{
			m_counterTrue--;
		}
	}

	// Write value to Wire.
	lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, m_connect.get(), value);
	events.emplace_back(std::move(i));

	// Save value to context.
	m_context->updateValues(now, { value });
	m_context->privateContext().putInt(0, m_counterFalse);
	m_context->privateContext().putInt(1, m_counterTrue);
	m_context->privateContext().putBool(2, m_counter);

	return events;
}

void lcsm::physical::Clock::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_connect = node;
}
