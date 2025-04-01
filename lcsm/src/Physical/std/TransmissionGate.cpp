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
	// If context already exists, reset it.
	if (m_context)
	{
		resetContext();
	}

	// Set and verify context.
	m_context = context;
	verifyContext();
}

void lcsm::physical::TransmissionGate::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::TransmissionGate::verifyContext()
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
	const lcsm::DataBits &srcC = m_context->getValue(3);
	const lcsm::Width desired = lcsm::Width::Bit1;
	if (base.width() != desired || srcA.width() != desired || srcB.width() != desired || srcC.width() != desired)
	{
		resetContext();
		throw std::logic_error("Bad value width!");
	}
}

void lcsm::physical::TransmissionGate::add(lcsm::Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::instruction_t type = instruction.type();

	if (target != this)
		throw std::logic_error("Target is not this object.");

	if (type != lcsm::InstructionType::WriteValue)
		throw std::logic_error("Bad instruction!");

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

	if (m_srcc == caller)
	{
		m_instantsSrcC.push_back(std::move(instruction));
	}

	throw std::logic_error("Bad port!");
}

void lcsm::physical::TransmissionGate::invoke(const lcsm::Timestamp &, std::deque< Event > &)
{
	// TODO: Implement me.
}

void lcsm::physical::TransmissionGate::connectBase(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_base = node;
}

void lcsm::physical::TransmissionGate::connectSrcA(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_srca = node;
}

void lcsm::physical::TransmissionGate::connectSrcB(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_srcb = node;
}

void lcsm::physical::TransmissionGate::connectSrcC(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node) noexcept
{
	m_srcc = node;
}
