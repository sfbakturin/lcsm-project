#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/Support/Ref.hpp>

#include <memory>
#include <utility>

sim::CG::CG(const sim::support::Ref< sim::CGObject > &target)
{
	m_root.SetTarget(target);
}

sim::CG::CG(sim::support::Ref< sim::CGObject > &&target)
{
	m_root.SetTarget(std::move(target));
}

sim::CG::CG(sim::CG &&other) noexcept : m_root(std::move(other.m_root)) {}

sim::CG &sim::CG::operator=(sim::CG &&other) noexcept
{
	if (this != &other)
		sim::CG(std::move(other)).Swap(*this);
	return *this;
}

void sim::CG::Swap(sim::CG &other) noexcept
{
	std::swap(m_root, other.m_root);
}

void sim::CG::SetTarget(const sim::support::Ref< CGObject > &target) noexcept
{
	m_root.SetTarget(target);
}

void sim::CG::SetTarget(sim::support::Ref< CGObject > &&target) noexcept
{
	m_root.SetTarget(std::move(target));
}

const sim::CGNode *sim::CG::GetRoot() const noexcept
{
	return std::addressof(m_root);
}

sim::CGNode *sim::CG::GetRoot() noexcept
{
	return std::addressof(m_root);
}
