#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Button.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Value.h>

#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Button::Button(lcsm::object_type_t objectType, bool activeOnPress) noexcept :
	lcsm::EvaluatorNode(objectType), m_activeOnPress(activeOnPress), m_wasPolluted(false)
{
}

lcsm::NodeType lcsm::physical::Button::nodeType() const noexcept
{
	return lcsm::NodeType::Static;
}

std::size_t lcsm::physical::Button::contextSize() const noexcept
{
	return 1;
}

std::size_t lcsm::physical::Button::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::Button::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
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

void lcsm::physical::Button::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::Button::verifyContext()
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

	const lcsm::DataBits &value = m_context->getValue();
	if (value.width() != lcsm::Width::Bit1)
	{
		resetContext();
		throw std::logic_error("Bad value's width!");
	}
}

void lcsm::physical::Button::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

	// Check, if target is this circuit.
	if (target != this)
	{
		throw std::logic_error("Bad this circuit in instruction!");
	}

	// Check, if caller is wire.
	if (m_wire != caller)
	{
		throw std::logic_error("Bad caller in instruction!");
	}

	// Check, if instruction type is supported. If it's WriteValue - ignore, otherwise, if PolluteValue, then remember
	// and return new clean value.
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

void lcsm::physical::Button::invoke(const lcsm::Timestamp &now, std::deque< lcsm::Event > &events)
{
	// Constants.
	lcsm::EvaluatorNode *target = m_wire.get();
	static const lcsm::DataBits T = lcsm::verilog::Bit::True;
	static const lcsm::DataBits F = lcsm::verilog::Bit::False;

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

	// Extract context.
	const lcsm::DataBits &databits = m_context->getValue();
	const lcsm::verilog::Value value = databits.value(0);
	lcsm::DataBits pushed;

	// Generate pushed value by bit in value.
	switch (value.bit())
	{
	// Undefined and False - is when we stopped to push the button.
	case lcsm::verilog::Undefined:
	case lcsm::verilog::False:
	{
		pushed = (m_activeOnPress ? F : T);
		break;
	}
	// True - is when we start to push the button.
	case lcsm::verilog::True:
	{
		pushed = (m_activeOnPress ? T : F);
		break;
	}
	}

	// Save context and generate event.
	m_context->updateValues(now, { pushed });

	// Create write value.
	events.push_back(lcsm::CreateWriteValueInstruction(this, target, std::move(pushed)));
}

void lcsm::physical::Button::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_wire = node;
}
