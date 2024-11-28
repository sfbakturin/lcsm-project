#include <lcsm/IR/CG.h>
#include <lcsm/IR/Event.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <utility>

lcsm::Event::Event(
	const lcsm::Instruction &instruction,
	const lcsm::support::PointerView< CGNode > &targetFrom,
	const lcsm::support::PointerView< CGNode > &targetTo,
	unsigned diff) noexcept : m_instruction(instruction), m_targetFrom(targetFrom), m_targetTo(targetTo), m_diff(diff)
{
}

lcsm::Event::Event(
	lcsm::Instruction &&instruction,
	const lcsm::support::PointerView< CGNode > &targetFrom,
	const lcsm::support::PointerView< CGNode > &targetTo,
	unsigned diff) noexcept :
	m_instruction(std::move(instruction)), m_targetFrom(targetFrom), m_targetTo(targetTo), m_diff(diff)
{
}

lcsm::Event::Event(const lcsm::Event &other) noexcept :
	m_instruction(other.m_instruction), m_targetFrom(other.m_targetFrom), m_targetTo(other.m_targetTo), m_diff(other.m_diff)
{
}

lcsm::Event::Event(lcsm::Event &&other) noexcept :
	m_instruction(std::move(other.m_instruction)), m_targetFrom(std::move(other.m_targetFrom)),
	m_targetTo(std::move(other.m_targetTo)), m_diff(other.m_diff)
{
}

lcsm::Event &lcsm::Event::operator=(const lcsm::Event &other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Event(other).swap(*this);
	return *this;
}

lcsm::Event &lcsm::Event::operator=(lcsm::Event &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Event(std::move(other)).swap(*this);
	return *this;
}

void lcsm::Event::swap(lcsm::Event &other) noexcept
{
	std::swap(m_instruction, other.m_instruction);
	std::swap(m_targetFrom, other.m_targetFrom);
	std::swap(m_targetTo, other.m_targetTo);
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

const lcsm::support::PointerView< lcsm::CGNode > &lcsm::Event::targetFrom() const noexcept
{
	return m_targetFrom;
}

lcsm::support::PointerView< lcsm::CGNode > &lcsm::Event::targetFrom() noexcept
{
	return m_targetFrom;
}

const lcsm::support::PointerView< lcsm::CGNode > &lcsm::Event::targetTo() const noexcept
{
	return m_targetTo;
}

lcsm::support::PointerView< lcsm::CGNode > &lcsm::Event::targetTo() noexcept
{
	return m_targetTo;
}

bool lcsm::Event::isInstant() const noexcept
{
	return m_diff == 0;
}

bool lcsm::Event::isFuture() const noexcept
{
	return m_diff != 0;
}

lcsm::Event lcsm::Event::toInstant() const noexcept
{
	return lcsm::CreateInstantEvent(m_instruction, m_targetFrom, m_targetTo);
}

unsigned lcsm::Event::diff() const noexcept
{
	return m_diff;
}

void lcsm::Event::setDiff(unsigned newDiff) noexcept
{
	m_diff = newDiff;
}

lcsm::Event lcsm::CreateInstantEvent(
	const lcsm::Instruction &instruction,
	const lcsm::support::PointerView< lcsm::CGNode > &targetFrom,
	const lcsm::support::PointerView< lcsm::CGNode > &targetTo) noexcept
{
	return lcsm::CreateFutureEvent(instruction, targetFrom, targetTo, 0);
}

lcsm::Event lcsm::CreateInstantEvent(
	lcsm::Instruction &&instruction,
	const lcsm::support::PointerView< lcsm::CGNode > &targetFrom,
	const lcsm::support::PointerView< lcsm::CGNode > &targetTo) noexcept
{
	return lcsm::CreateFutureEvent(std::move(instruction), targetFrom, targetTo, 0);
}

lcsm::Event lcsm::CreateFutureEvent(
	const lcsm::Instruction &instruction,
	const lcsm::support::PointerView< lcsm::CGNode > &targetFrom,
	const lcsm::support::PointerView< lcsm::CGNode > &targetTo,
	unsigned diff) noexcept
{
	return lcsm::Event(instruction, targetFrom, targetTo, diff);
}

lcsm::Event lcsm::CreateFutureEvent(
	lcsm::Instruction &&instruction,
	const lcsm::support::PointerView< lcsm::CGNode > &targetFrom,
	const lcsm::support::PointerView< lcsm::CGNode > &targetTo,
	unsigned diff) noexcept
{
	return lcsm::Event(std::move(instruction), targetFrom, targetTo, diff);
}
