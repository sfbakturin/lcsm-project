#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/Algorithm.hpp>

#include <utility>

lcsm::Event::Event(const lcsm::Instruction &instruction) noexcept : m_instruction(instruction) {}

lcsm::Event::Event(lcsm::Instruction &&instruction) noexcept : m_instruction(std::move(instruction)) {}

lcsm::Event::Event(const lcsm::Instruction &instruction, const lcsm::Timestamp &diff) noexcept :
	m_instruction(instruction), m_diff(diff)
{
}

lcsm::Event::Event(lcsm::Instruction &&instruction, const lcsm::Timestamp &diff) noexcept :
	m_instruction(std::move(instruction)), m_diff(diff)
{
}

lcsm::Event::Event(const lcsm::Instruction &instruction, lcsm::Timestamp &&diff) noexcept :
	m_instruction(instruction), m_diff(std::move(diff))
{
}

lcsm::Event::Event(lcsm::Instruction &&instruction, lcsm::Timestamp &&diff) noexcept :
	m_instruction(std::move(instruction)), m_diff(std::move(diff))
{
}

lcsm::Event::Event(const lcsm::Event &other) noexcept : m_instruction(other.m_instruction), m_diff(other.m_diff) {}

lcsm::Event::Event(lcsm::Event &&other) noexcept :
	m_instruction(std::move(other.m_instruction)), m_diff(std::move(other.m_diff))
{
}

lcsm::Event &lcsm::Event::operator=(const lcsm::Event &other) noexcept
{
	return lcsm::support::CopyAssign< lcsm::Event >(this, other);
}

lcsm::Event &lcsm::Event::operator=(lcsm::Event &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::Event >(this, std::move(other));
}

void lcsm::Event::swap(lcsm::Event &other) noexcept
{
	std::swap(m_instruction, other.m_instruction);
	std::swap(m_diff, other.m_diff);
}

const lcsm::Instruction &lcsm::Event::instruction() const noexcept
{
	return m_instruction;
}

lcsm::Instruction &lcsm::Event::instruction() noexcept
{
	return m_instruction;
}

bool lcsm::Event::isInstant() const noexcept
{
	return m_diff.isReset();
}

bool lcsm::Event::isFuture() const noexcept
{
	return !isInstant();
}

lcsm::Event lcsm::Event::toInstant() const noexcept
{
	return lcsm::Event(m_instruction);
}

const lcsm::Timestamp &lcsm::Event::diff() const noexcept
{
	return m_diff;
}
