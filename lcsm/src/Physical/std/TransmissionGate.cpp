#include <lcsm/LCSM.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/std/TransmissionGate.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::TransmissionGate::TransmissionGate(lcsm::object_type_t objectType) : lcsm::EvaluatorNode(objectType) {}

lcsm::NodeType lcsm::physical::TransmissionGate::nodeType() const noexcept
{
	return lcsm::NodeType::Dynamic;
}

std::size_t lcsm::physical::TransmissionGate::contextSize() const noexcept
{
	return 4;
}

std::size_t lcsm::physical::TransmissionGate::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::TransmissionGate::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	if (context->size() != contextSize() || context->privateSize() != privateContextSize())
	{
		throw std::logic_error("Bad context size!");
	}

	// If already contexted, reset old.
	if (m_context)
	{
		resetContext();
	}

	// Set contexted.
	m_context = context;
}

void lcsm::physical::TransmissionGate::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::TransmissionGate::addInstant(const lcsm::Instruction &instruction)
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

	if (caller == m_srcc)
	{
		m_instantsSrcC.push_back(instruction);
	}

	throw std::logic_error("Bad port!");
}

void lcsm::physical::TransmissionGate::addInstant(lcsm::Instruction &&instruction)
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

	if (caller == m_srcc)
	{
		m_instantsSrcC.push_back(std::move(instruction));
	}

	throw std::logic_error("Bad port!");
}

std::vector< lcsm::Event > lcsm::physical::TransmissionGate::invokeInstants(const lcsm::Timestamp &)
{
	// Generated events.
	std::vector< lcsm::Event > events;
	// TODO: Implement TransmissionGate/Transistor, when Verilog will be supported.
	return events;
}

void lcsm::physical::TransmissionGate::connectBase(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_base = node;
}

void lcsm::physical::TransmissionGate::connectSrcA(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_srca = node;
}

void lcsm::physical::TransmissionGate::connectSrcB(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_srcb = node;
}

void lcsm::physical::TransmissionGate::connectSrcC(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_srcc = node;
}
