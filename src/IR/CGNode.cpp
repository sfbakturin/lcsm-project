#include <sim/IR/CG.h>
#include <sim/IR/Instruction.h>
#include <sim/Support/PointerView.hpp>

#include <memory>
#include <utility>

sim::CGNode::CGNode(const sim::support::PointerView< sim::CGObject > &target) : m_target(target) {}

sim::CGNode::CGNode(sim::support::PointerView< sim::CGObject > &&target) : m_target(std::move(target)) {}

sim::CGNode::CGNode(const std::shared_ptr< sim::Instruction > &run) : m_run(run) {}

sim::CGNode::CGNode(std::shared_ptr< sim::Instruction > &&run) : m_run(std::move(run)) {}

sim::CGNode::CGNode(const sim::support::PointerView< sim::CGObject > &target, const std::shared_ptr< sim::Instruction > &run) :
	m_target(target), m_run(run)
{
}

sim::CGNode::CGNode(const sim::support::PointerView< sim::CGObject > &target, std::shared_ptr< sim::Instruction > &&run) :
	m_target(target), m_run(std::move(run))
{
}

sim::CGNode::CGNode(sim::support::PointerView< sim::CGObject > &&target, const std::shared_ptr< sim::Instruction > &run) :
	m_target(std::move(target)), m_run(run)
{
}

sim::CGNode::CGNode(sim::support::PointerView< sim::CGObject > &&target, std::shared_ptr< sim::Instruction > &&run) :
	m_target(std::move(target)), m_run(std::move(run))
{
}

sim::CGNode::CGNode(sim::CGNode &&other) noexcept :
	m_target(std::move(other.m_target)), m_run(std::move(other.m_run)), m_adjacent(std::move(other.m_adjacent))
{
}

sim::CGNode &sim::CGNode::operator=(sim::CGNode &&other) noexcept
{
	if (this != &other)
		sim::CGNode(std::move(other)).Swap(*this);
	return *this;
}

void sim::CGNode::Swap(sim::CGNode &other) noexcept
{
	std::swap(m_target, other.m_target);
	std::swap(m_run, other.m_run);
	std::swap(m_adjacent, other.m_adjacent);
}

const sim::Instruction *sim::CGNode::GetInstruction() const noexcept
{
	return m_run.get();
}

sim::Instruction *sim::CGNode::GetInstruction() noexcept
{
	return m_run.get();
}

const sim::CGObject *sim::CGNode::GetTarget() const noexcept
{
	return m_target.Ptr();
}

sim::CGObject *sim::CGNode::GetTarget() noexcept
{
	return m_target.Ptr();
}

const std::vector< sim::CGNode > &sim::CGNode::GetConnections() const noexcept
{
	return m_adjacent;
}

std::vector< sim::CGNode > &sim::CGNode::GetConnections() noexcept
{
	return m_adjacent;
}

void sim::CGNode::SetInstruction(const std::shared_ptr< sim::Instruction > &run) noexcept
{
	m_run = run;
}

void sim::CGNode::SetInstruction(std::shared_ptr< sim::Instruction > &&run) noexcept
{
	m_run = std::move(run);
}

void sim::CGNode::SetTarget(const sim::support::PointerView< sim::CGObject > &target) noexcept
{
	m_target = target;
}

void sim::CGNode::SetTarget(sim::support::PointerView< sim::CGObject > &&target) noexcept
{
	m_target = std::move(target);
}

sim::CGNode *sim::CGNode::AddConnection(sim::CGNode &&connection)
{
	m_adjacent.push_back(std::move(connection));
	return std::addressof(m_adjacent.back());
}
