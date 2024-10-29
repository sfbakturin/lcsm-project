#include <lcsm/IR/CG.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <utility>

lcsm::CGEdge::CGEdge() : m_timer(0) {}

lcsm::CGEdge::CGEdge(const lcsm::CGObjectView &targetFrom, const lcsm::CGObjectView &targetTo, const std::shared_ptr< lcsm::Instruction > &run) :
	m_targetFrom(targetFrom), m_targetTo(targetTo), m_run(run), m_timer(0)
{
}

lcsm::CGEdge::CGEdge(const lcsm::CGObjectView &targetFrom, const lcsm::CGObjectView &targetTo, std::shared_ptr< lcsm::Instruction > &&run) :
	m_targetFrom(targetFrom), m_targetTo(targetTo), m_run(std::move(run)), m_timer(0)
{
}

lcsm::CGEdge::CGEdge(lcsm::CGEdge &&other) noexcept :
	m_targetFrom(std::move(other.m_targetFrom)), m_targetTo(std::move(other.m_targetTo)), m_run(std::move(other.m_run)),
	m_timer(other.m_timer)
{
}

lcsm::CGEdge &lcsm::CGEdge::operator=(lcsm::CGEdge &&other) noexcept
{
	if (this != &other)
		lcsm::CGEdge(std::move(other)).swap(*this);
	return *this;
}

void lcsm::CGEdge::swap(lcsm::CGEdge &other) noexcept
{
	std::swap(m_targetFrom, other.m_targetFrom);
	std::swap(m_targetTo, other.m_targetTo);
	std::swap(m_run, other.m_run);
}

lcsm::Instruction *lcsm::CGEdge::instruction() noexcept
{
	return m_run.get();
}

const lcsm::Instruction *lcsm::CGEdge::instruction() const noexcept
{
	return m_run.get();
}

lcsm::CGObject *lcsm::CGEdge::targetFrom() noexcept
{
	return m_targetFrom.ptr();
}

const lcsm::CGObject *lcsm::CGEdge::targetFrom() const noexcept
{
	return m_targetFrom.cptr();
}

lcsm::CGObject *lcsm::CGEdge::targetTo() noexcept
{
	return m_targetTo.ptr();
}

const lcsm::CGObject *lcsm::CGEdge::targetTo() const noexcept
{
	return m_targetTo.cptr();
}

void lcsm::CGEdge::setInstruction(const std::shared_ptr< lcsm::Instruction > &run) noexcept
{
	m_run = run;
}

void lcsm::CGEdge::setInstruction(std::shared_ptr< lcsm::Instruction > &&run) noexcept
{
	m_run = std::move(run);
}

void lcsm::CGEdge::setTargetFrom(const CGObjectView &targetFrom) noexcept
{
	m_targetFrom = targetFrom;
}

void lcsm::CGEdge::setTargetFrom(CGObjectView &&targetFrom) noexcept
{
	m_targetFrom = std::move(targetFrom);
}

void lcsm::CGEdge::setTargetTo(const CGObjectView &targetTo) noexcept
{
	m_targetTo = targetTo;
}

void lcsm::CGEdge::setTargetTo(CGObjectView &&targetTo) noexcept
{
	m_targetTo = std::move(targetTo);
}
