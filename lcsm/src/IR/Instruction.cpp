#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Instruction.h>

#include <memory>
#include <utility>

lcsm::Instruction::Instruction(lcsm::InstructionType type, lcsm::CGObject *caller, lcsm::CGObject *target) noexcept :
	m_type(type), m_caller(caller), m_target(target)
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

const lcsm::CGObject *lcsm::Instruction::caller() const noexcept
{
	return m_caller;
}

lcsm::CGObject *lcsm::Instruction::caller() noexcept
{
	return m_caller;
}

const lcsm::CGObject *lcsm::Instruction::target() const noexcept
{
	return m_target;
}

lcsm::CGObject *lcsm::Instruction::target() noexcept
{
	return m_target;
}

bool lcsm::Instruction::isWrite() const noexcept
{
	return m_type == lcsm::InstructionType::WriteValue;
}

bool lcsm::Instruction::isBroadcast() const noexcept
{
	return m_type == lcsm::InstructionType::BroadcastValue;
}

bool lcsm::Instruction::isRequest() const noexcept
{
	return m_type == lcsm::InstructionType::Request;
}

bool lcsm::Instruction::isUpdateState() const noexcept
{
	return m_type == lcsm::InstructionType::UpdateState;
}

lcsm::Instruction lcsm::CreateWriteValueInstruction(lcsm::CGObject *caller, lcsm::CGObject *target) noexcept
{
	return lcsm::Instruction(lcsm::InstructionType::WriteValue, caller, target);
}

lcsm::Instruction lcsm::CreateBroadcastValueInstruction(lcsm::CGObject *caller, lcsm::CGObject *target) noexcept
{
	return lcsm::Instruction(lcsm::InstructionType::BroadcastValue, caller, target);
}

lcsm::Instruction lcsm::CreateRequestInstruction(lcsm::CGObject *caller, lcsm::CGObject *target) noexcept
{
	return lcsm::Instruction(lcsm::InstructionType::Request, caller, target);
}

lcsm::Instruction lcsm::CreateUpdateStateInstruction(lcsm::CGObject *caller, lcsm::CGObject *target) noexcept
{
	return lcsm::Instruction(lcsm::InstructionType::UpdateState, caller, target);
}
