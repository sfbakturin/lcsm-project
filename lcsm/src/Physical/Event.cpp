#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <utility>

lcsm::Event::Event(
	const lcsm::Instruction &instruction,
	const lcsm::support::PointerView< EvaluatorNode > &targetFrom,
	const lcsm::support::PointerView< EvaluatorNode > &targetTo,
	const lcsm::Timestamp &diff) noexcept :
	m_instruction(instruction), m_targetFrom(targetFrom), m_targetTo(targetTo), m_diff(diff)
{
}

lcsm::Event::Event(
	lcsm::Instruction &&instruction,
	const lcsm::support::PointerView< EvaluatorNode > &targetFrom,
	const lcsm::support::PointerView< EvaluatorNode > &targetTo,
	const lcsm::Timestamp &diff) noexcept :
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

const lcsm::support::PointerView< lcsm::EvaluatorNode > &lcsm::Event::targetFrom() const noexcept
{
	return m_targetFrom;
}

lcsm::support::PointerView< lcsm::EvaluatorNode > &lcsm::Event::targetFrom() noexcept
{
	return m_targetFrom;
}

const lcsm::support::PointerView< lcsm::EvaluatorNode > &lcsm::Event::targetTo() const noexcept
{
	return m_targetTo;
}

lcsm::support::PointerView< lcsm::EvaluatorNode > &lcsm::Event::targetTo() noexcept
{
	return m_targetTo;
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
	return lcsm::CreateInstantEvent(m_instruction, m_targetFrom, m_targetTo);
}

const lcsm::Timestamp &lcsm::Event::diff() const noexcept
{
	return m_diff;
}

void lcsm::Event::setDiff(const lcsm::Timestamp &newDiff) noexcept
{
	m_diff = newDiff;
}

void lcsm::Event::setDiff(lcsm::Timestamp &&newDiff) noexcept
{
	m_diff = std::move(newDiff);
}

lcsm::Event lcsm::CreateInstantEvent(
	const lcsm::Instruction &instruction,
	const lcsm::support::PointerView< lcsm::EvaluatorNode > &targetFrom,
	const lcsm::support::PointerView< lcsm::EvaluatorNode > &targetTo) noexcept
{
	return lcsm::CreateFutureEvent(instruction, targetFrom, targetTo, lcsm::Timestamp::RESET);
}

lcsm::Event lcsm::CreateInstantEvent(
	lcsm::Instruction &&instruction,
	const lcsm::support::PointerView< lcsm::EvaluatorNode > &targetFrom,
	const lcsm::support::PointerView< lcsm::EvaluatorNode > &targetTo) noexcept
{
	return lcsm::CreateFutureEvent(std::move(instruction), targetFrom, targetTo, lcsm::Timestamp::RESET);
}

lcsm::Event lcsm::CreateFutureEvent(
	const lcsm::Instruction &instruction,
	const lcsm::support::PointerView< lcsm::EvaluatorNode > &targetFrom,
	const lcsm::support::PointerView< lcsm::EvaluatorNode > &targetTo,
	const lcsm::Timestamp &diff) noexcept
{
	return lcsm::Event(instruction, targetFrom, targetTo, diff);
}

lcsm::Event lcsm::CreateFutureEvent(
	lcsm::Instruction &&instruction,
	const lcsm::support::PointerView< lcsm::EvaluatorNode > &targetFrom,
	const lcsm::support::PointerView< lcsm::EvaluatorNode > &targetTo,
	const lcsm::Timestamp &diff) noexcept
{
	return lcsm::Event(std::move(instruction), targetFrom, targetTo, diff);
}
