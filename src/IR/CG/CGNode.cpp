#include <sim/IR/CG.h>
#include <sim/IR/Instruction.h>
#include <sim/Support/PointerView.hpp>

#include <memory>
#include <utility>

sim::CGNode::CGNode() : m_timer(0) {}

sim::CGNode::CGNode(sim::CGNode &&other) noexcept :
	m_targetFrom(std::move(other.m_targetFrom)),
	m_targetTo(std::move(other.m_targetTo)), m_run(std::move(other.m_run)),
	m_adjacent(std::move(other.m_adjacent)), m_timer(other.m_timer)
{
}

sim::CGNode &sim::CGNode::operator=(sim::CGNode &&other) noexcept
{
	if (this != &other)
		sim::CGNode(std::move(other)).swap(*this);
	return *this;
}

void sim::CGNode::swap(sim::CGNode &other) noexcept
{
	std::swap(m_targetFrom, other.m_targetFrom);
	std::swap(m_targetTo, other.m_targetTo);
	std::swap(m_run, other.m_run);
	std::swap(m_adjacent, other.m_adjacent);
}

sim::Instruction *sim::CGNode::instruction() noexcept
{
	return m_run.get();
}

const sim::Instruction *sim::CGNode::instruction() const noexcept
{
	return m_run.get();
}

sim::CGObject *sim::CGNode::targetFrom() noexcept
{
	return m_targetFrom.Ptr();
}

const sim::CGObject *sim::CGNode::targetFrom() const noexcept
{
	return m_targetFrom.Ptr();
}

sim::CGObject *sim::CGNode::targetTo() noexcept
{
	return m_targetTo.Ptr();
}

const sim::CGObject *sim::CGNode::targetTo() const noexcept
{
	return m_targetTo.Ptr();
}

std::vector< sim::CGNode > &sim::CGNode::connections() noexcept
{
	return m_adjacent;
}

const std::vector< sim::CGNode > &sim::CGNode::connections() const noexcept
{
	return m_adjacent;
}

void sim::CGNode::setInstruction(const std::shared_ptr< sim::Instruction > &run) noexcept
{
	m_run = run;
}

void sim::CGNode::setInstruction(std::shared_ptr< sim::Instruction > &&run) noexcept
{
	m_run = std::move(run);
}

void sim::CGNode::setTargetFrom(const CGObjectView &targetFrom) noexcept
{
	m_targetFrom = targetFrom;
}

void sim::CGNode::setTargetFrom(CGObjectView &&targetFrom) noexcept
{
	m_targetFrom = std::move(targetFrom);
}

void sim::CGNode::setTargetTo(const CGObjectView &targetTo) noexcept
{
	m_targetTo = targetTo;
}

void sim::CGNode::setTargetTo(CGObjectView &&targetTo) noexcept
{
	m_targetTo = std::move(targetTo);
}

sim::CGNode *sim::CGNode::addChild(sim::CGNode &&connection)
{
	m_adjacent.push_back(std::move(connection));
	return std::addressof(m_adjacent.back());
}
