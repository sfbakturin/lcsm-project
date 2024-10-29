#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/Support/PointerView.hpp>

#include <utility>
#include <vector>

lcsm::CG::CG(lcsm::CG &&other) noexcept : m_roots(std::move(other.m_roots)) {}

lcsm::CG &lcsm::CG::operator=(lcsm::CG &&other) noexcept
{
	if (this != &other)
		lcsm::CG(std::move(other)).swap(*this);
	return *this;
}

void lcsm::CG::swap(lcsm::CG &other) noexcept
{
	std::swap(m_roots, other.m_roots);
}

void lcsm::CG::addRoot(const lcsm::CGNodeView &node)
{
	m_roots.push_back(node);
}

void lcsm::CG::addRoot(lcsm::CGNodeView &&node)
{
	m_roots.push_back(std::move(node));
}
