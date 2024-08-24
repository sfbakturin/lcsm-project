#include <sim/IR/CG.h>
#include <sim/IR/Instruction.h>
#include <sim/Support/PointerView.hpp>

#include <memory>
#include <utility>

sim::CGEdge::CGEdge() : m_timer(0) {}

sim::CGEdge::CGEdge(const sim::CGObjectView &targetFrom,
					const sim::CGObjectView &targetTo,
					const std::shared_ptr< sim::Instruction > &run) :
	m_targetFrom(targetFrom), m_targetTo(targetTo), m_run(run), m_timer(0)
{
}

sim::CGEdge::CGEdge(const sim::CGObjectView &targetFrom,
					const sim::CGObjectView &targetTo,
					std::shared_ptr< sim::Instruction > &&run) :
	m_targetFrom(targetFrom), m_targetTo(targetTo), m_run(std::move(run)), m_timer(0)
{
}

sim::CGEdge::CGEdge(sim::CGEdge &&other) noexcept :
	m_targetFrom(std::move(other.m_targetFrom)), m_targetTo(std::move(other.m_targetTo)),
	m_run(std::move(other.m_run)), m_timer(other.m_timer)
{
}

sim::CGEdge &sim::CGEdge::operator=(sim::CGEdge &&other) noexcept
{
	if (this != &other)
		sim::CGEdge(std::move(other)).swap(*this);
	return *this;
}

void sim::CGEdge::swap(sim::CGEdge &other) noexcept
{
	std::swap(m_targetFrom, other.m_targetFrom);
	std::swap(m_targetTo, other.m_targetTo);
	std::swap(m_run, other.m_run);
}

sim::Instruction *sim::CGEdge::instruction() noexcept
{
	return m_run.get();
}

const sim::Instruction *sim::CGEdge::instruction() const noexcept
{
	return m_run.get();
}

sim::CGObject *sim::CGEdge::targetFrom() noexcept
{
	return m_targetFrom.Ptr();
}

const sim::CGObject *sim::CGEdge::targetFrom() const noexcept
{
	return m_targetFrom.Ptr();
}

sim::CGObject *sim::CGEdge::targetTo() noexcept
{
	return m_targetTo.Ptr();
}

const sim::CGObject *sim::CGEdge::targetTo() const noexcept
{
	return m_targetTo.Ptr();
}

void sim::CGEdge::setInstruction(const std::shared_ptr< sim::Instruction > &run) noexcept
{
	m_run = run;
}

void sim::CGEdge::setInstruction(std::shared_ptr< sim::Instruction > &&run) noexcept
{
	m_run = std::move(run);
}

void sim::CGEdge::setTargetFrom(const CGObjectView &targetFrom) noexcept
{
	m_targetFrom = targetFrom;
}

void sim::CGEdge::setTargetFrom(CGObjectView &&targetFrom) noexcept
{
	m_targetFrom = std::move(targetFrom);
}

void sim::CGEdge::setTargetTo(const CGObjectView &targetTo) noexcept
{
	m_targetTo = targetTo;
}

void sim::CGEdge::setTargetTo(CGObjectView &&targetTo) noexcept
{
	m_targetTo = std::move(targetTo);
}
