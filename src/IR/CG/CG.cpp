#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/Support/PointerView.hpp>

#include <utility>
#include <vector>

sim::CG::CG(sim::CG &&other) noexcept : m_roots(std::move(other.m_roots)) {}

sim::CG &sim::CG::operator=(sim::CG &&other) noexcept
{
	if (this != &other)
		sim::CG(std::move(other)).swap(*this);
	return *this;
}

void sim::CG::swap(sim::CG &other) noexcept
{
	std::swap(m_roots, other.m_roots);
}

void sim::CG::addRoot(const sim::CGNodeView &node)
{
	m_roots.push_back(node);
}

void sim::CG::addRoot(sim::CGNodeView &&node)
{
	m_roots.push_back(std::move(node));
}
