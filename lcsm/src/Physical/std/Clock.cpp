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

#include <deque>
#include <stdexcept>
#include <vector>

#define UNUSED(X) ((void)(X))

static const lcsm::DataBits T{ lcsm::Width::Bit1, lcsm::verilog::Bit::True };
static const lcsm::DataBits F{ lcsm::Width::Bit1, lcsm::verilog::Bit::False };

lcsm::physical::Clock::Clock(lcsm::object_type_t objectType, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset) noexcept :
	lcsm::EvaluatorNode(objectType), m_highDuration(highDuration), m_lowDuration(lowDuration), m_phaseOffset(phaseOffset),
	m_counterFalse(m_lowDuration), m_counterTrue(m_highDuration), m_counter(true), m_wasPolluted(false)
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
	// If already context exists, reset old, set and verify new context and extract counters.
	// Otherwise, set and verify context without extracting counters.
	if (m_context)
	{
		resetContext();
		m_context = context;
		verifyContext();
		m_counterFalse = m_context->privateContext().asInt(0);
		m_counterTrue = m_context->privateContext().asInt(1);
		m_counter = m_context->privateContext().asBool(2);
	}
	else
	{
		m_context = context;
		verifyContext();
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

void lcsm::physical::Clock::verifyContext()
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
	if (value.width() != lcsm::Width::Bit1)
	{
		resetContext();
		throw std::logic_error("Bad value width!");
	}
}

void lcsm::physical::Clock::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

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

	throw std::logic_error("Bad instruction!");
}

void lcsm::physical::Clock::invoke(const lcsm::Timestamp &now, std::deque< lcsm::Event > &events)
{
	// If there is no updates, then put 1bit-Strong-False to context. Otherwise, read from context.
	lcsm::DataBits value;
	UNUSED(m_phaseOffset);

	// Special case: when any of counters equals zero always.
	if (m_highDuration == 0)
	{
		value = F;
		goto l_write;
	}
	else if (m_lowDuration == 0)
	{
		value = T;
		goto l_write;
	}

	// Check counter of value's bit (0 - False, 1 - True), if equals zero, then change value and reset counter,
	// otherwise, decrement and continue.
	if (m_counter)
	{
		if (m_counterTrue == 0)
		{
			value = F;
			m_counterTrue = m_highDuration;
			m_counterFalse--;
			m_counter = false;
		}
		else
		{
			m_counterTrue--;
			value = T;
		}
	}
	else
	{
		if (m_counterFalse == 0)
		{
			value = T;
			m_counterFalse = m_lowDuration;
			m_counterTrue--;
			m_counter = true;
		}
		else
		{
			m_counterFalse--;
			value = F;
		}
	}

l_write:
	// Write value to Wire.
	events.emplace_back(lcsm::CreateWriteValueInstruction(this, m_connect.get(), value));

	// Save value to context.
	m_context->updateValues(now, { value });
	m_context->privateContext().putInt(0, m_counterFalse);
	m_context->privateContext().putInt(1, m_counterTrue);
	m_context->privateContext().putBool(2, m_counter);
}

void lcsm::physical::Clock::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_connect = node;
}
