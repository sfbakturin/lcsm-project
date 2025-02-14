#include <lcsm/LCSM.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/Algorithm.hpp>

#include <utility>

lcsm::Instruction::Instruction(lcsm::InstructionType type, lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target) noexcept
	: m_type(type), m_caller(caller), m_target(target)
{
}

lcsm::Instruction::Instruction(const Instruction &other) noexcept :
	m_type(other.m_type), m_caller(other.m_caller), m_target(other.m_target), m_value(other.m_value)
{
}

lcsm::Instruction::Instruction(Instruction &&other) noexcept :
	m_type(other.m_type), m_caller(other.m_caller), m_target(other.m_target), m_value(std::move(other.m_value))
{
	other.m_caller = nullptr;
	other.m_target = nullptr;
}

lcsm::Instruction &lcsm::Instruction::operator=(const lcsm::Instruction &other) noexcept
{
	return lcsm::support::CopyAssign< lcsm::Instruction >(this, other);
}

lcsm::Instruction &lcsm::Instruction::operator=(lcsm::Instruction &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::Instruction >(this, std::move(other));
}

void lcsm::Instruction::swap(lcsm::Instruction &other) noexcept
{
	std::swap(m_type, other.m_type);
	std::swap(m_caller, other.m_caller);
	std::swap(m_target, other.m_target);
	std::swap(m_value, other.m_value);
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

const lcsm::DataBits &lcsm::Instruction::value() const noexcept
{
	return m_value;
}

void lcsm::Instruction::setValue(const lcsm::DataBits &value) noexcept
{
	m_value = value;
}

lcsm::Instruction lcsm::CreateWriteValueInstruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target) noexcept
{
	return lcsm::Instruction(lcsm::InstructionType::WriteValue, caller, target);
}
