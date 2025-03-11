#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/std/Button.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Value.h>

#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::Button::Button(lcsm::object_type_t objectType, bool activeOnPress) :
	lcsm::EvaluatorNode(objectType), m_activeOnPress(activeOnPress)
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

void lcsm::physical::Button::addInstant(const lcsm::Instruction &instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType type = instruction.type();

	if (target != this || m_wire != caller || type != lcsm::InstructionType::WriteValue)
	{
		throw std::logic_error("Bad instruction!");
	}
}

void lcsm::physical::Button::addInstant(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType type = instruction.type();

	if (target != this || m_wire != caller || type != lcsm::InstructionType::WriteValue)
	{
		throw std::logic_error("Bad instruction!");
	}
}

std::vector< lcsm::Event > lcsm::physical::Button::invokeInstants(const lcsm::Timestamp &now)
{
	// Constants.
	const lcsm::InstructionType type = lcsm::InstructionType::WriteValue;
	lcsm::EvaluatorNode *target = m_wire.get();
	static const lcsm::DataBits T = lcsm::verilog::Bit::True;
	static const lcsm::DataBits F = lcsm::verilog::Bit::False;

	// Generated events.
	std::vector< lcsm::Event > events;

	// Extract context.
	const lcsm::DataBits &databits = m_context->getValue();
	const lcsm::verilog::Value &value = databits.value(0);
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
	lcsm::Instruction I = { type, this, target, std::move(pushed) };
	events.emplace_back(std::move(I));

	return events;
}

void lcsm::physical::Button::connect(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_wire = node;
}
