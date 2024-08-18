#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/Support/PointerView.hpp>

#include <memory>
#include <utility>

sim::CG::CG(sim::CG &&other) noexcept : m_root(std::move(other.m_root)) {}

sim::CG &sim::CG::operator=(sim::CG &&other) noexcept
{
	if (this != &other)
	{
		sim::CG(std::move(other)).swap(*this);
	}
	return *this;
}

void sim::CG::swap(sim::CG &other) noexcept
{
	std::swap(m_root, other.m_root);
}

sim::CGNode *sim::CG::root() noexcept
{
	return std::addressof(m_root);
}

const sim::CGNode *sim::CG::root() const noexcept
{
	return std::addressof(m_root);
}
