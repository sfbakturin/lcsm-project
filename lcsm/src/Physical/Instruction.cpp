#include <lcsm/LCSM.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>

#include <memory>
#include <utility>

lcsm::Instruction::Instruction(lcsm::InstructionType type, lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target) noexcept
	: m_type(type), m_caller(caller), m_target(target)
{
}

lcsm::Instruction::Instruction(const Instruction &other) noexcept :
	m_type(other.m_type), m_caller(other.m_caller), m_target(other.m_target)
{
}

lcsm::Instruction::Instruction(Instruction &&other) noexcept :
	m_type(other.m_type), m_caller(other.m_caller), m_target(other.m_target)
{
	other.m_caller = nullptr;
	other.m_target = nullptr;
}

lcsm::Instruction &lcsm::Instruction::operator=(const lcsm::Instruction &other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Instruction(other).swap(*this);
	return *this;
}

lcsm::Instruction &lcsm::Instruction::operator=(lcsm::Instruction &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Instruction(other).swap(*this);
	return *this;
}

void lcsm::Instruction::swap(lcsm::Instruction &other) noexcept
{
	std::swap(m_type, other.m_type);
	std::swap(m_caller, other.m_caller);
	std::swap(m_target, other.m_target);
}

lcsm::InstructionType lcsm::Instruction::type() const noexcept
{
	return m_type;
}

const lcsm::EvaluatorNode *lcsm::Instruction::caller() const noexcept
{
	return m_caller;
}

lcsm::EvaluatorNode *lcsm::Instruction::caller() noexcept
{
	return m_caller;
}

const lcsm::EvaluatorNode *lcsm::Instruction::target() const noexcept
{
	return m_target;
}

lcsm::EvaluatorNode *lcsm::Instruction::target() noexcept
{
	return m_target;
}

lcsm::Instruction lcsm::CreateWriteValueInstruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target) noexcept
{
	return lcsm::Instruction(lcsm::InstructionType::WriteValue, caller, target);
}

lcsm::Instruction lcsm::CreateRequestInstruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target) noexcept
{
	return lcsm::Instruction(lcsm::InstructionType::RequestUpdate, caller, target);
}

lcsm::Instruction lcsm::CreateUpdateStateInstruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target) noexcept
{
	return lcsm::Instruction(lcsm::InstructionType::UpdateState, caller, target);
}
