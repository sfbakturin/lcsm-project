#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Event.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <utility>
#include <vector>

lcsm::CGNode::CGNode(lcsm::CGNodeType nodeType) : m_nodeType(nodeType) {}

lcsm::CGNode::CGNode(lcsm::CGNode &&other) noexcept :
	m_nodeType(other.m_nodeType), m_children(std::move(other.m_children))
{
}

lcsm::CGNode &lcsm::CGNode::operator=(lcsm::CGNode &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::CGNode(std::move(other)).swap(*this);
	return *this;
}

void lcsm::CGNode::swap(lcsm::CGNode &other) noexcept
{
	std::swap(m_children, other.m_children);
}

lcsm::CGNodeType lcsm::CGNode::nodeType() const noexcept
{
	return m_nodeType;
}

bool lcsm::CGNode::isStatic() const noexcept
{
	return m_nodeType == lcsm::CGNodeType::Static;
}

bool lcsm::CGNode::isFast() const noexcept
{
	return m_nodeType == lcsm::CGNodeType::Fast;
}

bool lcsm::CGNode::isComposite() const noexcept
{
	return m_nodeType == lcsm::CGNodeType::Composite;
}

bool lcsm::CGNode::isDynamic() const noexcept
{
	return m_nodeType == lcsm::CGNodeType::Dynamic;
}

void lcsm::CGNode::pushBackChild(const lcsm::support::PointerView< CGNode > &child)
{
	m_children.push_back(child);
}

void lcsm::CGNode::pushBackChild(lcsm::support::PointerView< CGNode > &&child)
{
	m_children.push_back(std::move(child));
}
