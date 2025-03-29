#include <lcsm/LCSM.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/Algorithm.hpp>

#include <stdexcept>
#include <utility>

lcsm::Instruction::Instruction(lcsm::EvaluatorNode *caller, lcsm::instruction_t type) noexcept :
	m_caller(caller), m_target(nullptr), m_type(type), m_valued(false)
{
}

lcsm::Instruction::Instruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target, lcsm::instruction_t type) noexcept
	: m_caller(caller), m_target(target), m_type(type), m_valued(false)
{
}

lcsm::Instruction::Instruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target, lcsm::instruction_t type, const lcsm::DataBits &databits) noexcept
	: m_caller(caller), m_target(target), m_type(type), m_databits(databits), m_valued(true)
{
}

lcsm::Instruction::Instruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target, lcsm::instruction_t type, lcsm::DataBits &&databits) noexcept
	: m_caller(caller), m_target(target), m_type(type), m_databits(std::move(databits)), m_valued(true)
{
}

lcsm::Instruction::Instruction(const lcsm::Instruction &other) noexcept :
	m_caller(other.m_caller), m_target(other.m_target), m_type(other.m_type), m_databits(other.m_databits),
	m_valued(other.m_valued)
{
}

lcsm::Instruction::Instruction(lcsm::Instruction &&other) noexcept :
	m_caller(other.m_caller), m_target(other.m_target), m_type(other.m_type), m_databits(std::move(other.m_databits)),
	m_valued(other.m_valued)
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
	std::swap(m_caller, other.m_caller);
	std::swap(m_target, other.m_target);
	std::swap(m_type, other.m_type);
	std::swap(m_databits, other.m_databits);
	std::swap(m_valued, other.m_valued);
}

lcsm::instruction_t lcsm::Instruction::type() const noexcept
{
	return m_type;
}

bool lcsm::Instruction::operator==(lcsm::instruction_t type) const noexcept
{
	return m_type == type;
}

bool lcsm::Instruction::isInstruction() const noexcept
{
	return lcsm::InstructionType::WriteValue <= m_type && m_type < lcsm::InstructionType::LastInstructionType;
}

bool lcsm::Instruction::isSimulatorInstruction() const noexcept
{
	return lcsm::SimulatorInstructionType::PolluteCircuit <= m_type && m_type < lcsm::SimulatorInstructionType::LastSimulatorInstructionType;
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

bool lcsm::Instruction::valued() const noexcept
{
	return m_valued;
}

const lcsm::DataBits &lcsm::Instruction::value() const
{
	if (valued())
	{
		return m_databits;
	}
	throw std::logic_error("No value in instruction!");
}

lcsm::DataBits &lcsm::Instruction::value()
{
	if (valued())
	{
		return m_databits;
	}
	throw std::logic_error("No value in instruction!");
}

lcsm::Instruction
	lcsm::CreateWriteValueInstruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target, const lcsm::DataBits &value) noexcept
{
	return lcsm::Instruction(caller, target, lcsm::InstructionType::WriteValue, value);
}

lcsm::Instruction lcsm::CreateWriteValueInstruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target, lcsm::DataBits &&value) noexcept
{
	return lcsm::Instruction(caller, target, lcsm::InstructionType::WriteValue, std::move(value));
}

lcsm::Instruction lcsm::CreatePolluteValueInstruction(lcsm::EvaluatorNode *caller, lcsm::EvaluatorNode *target) noexcept
{
	return lcsm::Instruction(caller, target, lcsm::InstructionType::PolluteValue);
}

lcsm::Instruction lcsm::CreatePolluteCircuitSimulatorInstruction(lcsm::EvaluatorNode *caller) noexcept
{
	return lcsm::Instruction(caller, lcsm::SimulatorInstructionType::PolluteCircuit);
}
