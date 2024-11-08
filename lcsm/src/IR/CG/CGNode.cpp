#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <utility>
#include <vector>

lcsm::CGNode::CGNode(const lcsm::CGObjectView &target) : m_target(target) {}

lcsm::CGNode::CGNode(lcsm::CGObjectView &&target) : m_target(std::move(target)) {}

lcsm::CGNode::CGNode(lcsm::CGNode &&other) noexcept :
	m_target(std::move(other.m_target)), m_children(std::move(other.m_children))
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
	std::swap(m_target, other.m_target);
	std::swap(m_children, other.m_children);
}

lcsm::CGNodeType lcsm::CGNode::nodeType() const noexcept
{
	return lcsm::CGNodeType::Unknown;
}

bool lcsm::CGNode::isStatic() const noexcept
{
	return nodeType() == lcsm::CGNodeType::Static;
}

bool lcsm::CGNode::isFast() const noexcept
{
	return nodeType() == lcsm::CGNodeType::Fast;
}

bool lcsm::CGNode::isComposite() const noexcept
{
	return nodeType() == lcsm::CGNodeType::Composite;
}

bool lcsm::CGNode::isDynamic() const noexcept
{
	return nodeType() == lcsm::CGNodeType::Dynamic;
}

lcsm::CGStaticNode *lcsm::CGNode::asStatic() noexcept
{
	return nullptr;
}

const lcsm::CGStaticNode *lcsm::CGNode::asStatic() const noexcept
{
	return nullptr;
}

lcsm::CGFastNode *lcsm::CGNode::asFast() noexcept
{
	return nullptr;
}

const lcsm::CGFastNode *lcsm::CGNode::asFast() const noexcept
{
	return nullptr;
}

lcsm::CGCompositeNode *lcsm::CGNode::asComposite() noexcept
{
	return nullptr;
}

const lcsm::CGCompositeNode *lcsm::CGNode::asComposite() const noexcept
{
	return nullptr;
}

lcsm::CGDynamicNode *lcsm::CGNode::asDynamic() noexcept
{
	return nullptr;
}

const lcsm::CGDynamicNode *lcsm::CGNode::asDynamic() const noexcept
{
	return nullptr;
}

lcsm::CGObjectView &lcsm::CGNode::target() noexcept
{
	return m_target;
}

const lcsm::CGObjectView &lcsm::CGNode::target() const noexcept
{
	return m_target;
}

void lcsm::CGNode::setTarget(const lcsm::CGObjectView &target) noexcept
{
	m_target = target;
}

void lcsm::CGNode::setTarget(lcsm::CGObjectView &&target) noexcept
{
	m_target = std::move(target);
}

std::vector< lcsm::support::PointerView< lcsm::CGNode > > &lcsm::CGNode::children() noexcept
{
	return m_children;
}

const std::vector< lcsm::support::PointerView< lcsm::CGNode > > &lcsm::CGNode::children() const noexcept
{
	return m_children;
}

void lcsm::CGNode::pushBackChild(const lcsm::support::PointerView< CGNode > &child)
{
	m_children.push_back(child);
}

void lcsm::CGNode::pushBackChild(lcsm::support::PointerView< CGNode > &&child)
{
	m_children.push_back(std::move(child));
}

/* DEPRECATED
lcsm::CGNode::CGNode(const lcsm::CGObjectView &object) : m_target(object) {}

lcsm::CGNode::CGNode(lcsm::CGObjectView &&object) : m_target(std::move(object)) {}

lcsm::CGNode::CGNode(lcsm::CGObject *object) : m_target(object) {}

lcsm::CGNode::CGNode(lcsm::CGNode &&other) noexcept :
	m_target(std::move(other.m_target)), m_instructions(std::move(other.m_instructions))
{
}

lcsm::CGNode &lcsm::CGNode::operator=(lcsm::CGNode &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::CGNode(std::move(other)).swap(*this);
	return *this;
}

lcsm::NodeT lcsm::CGNode::T() const noexcept
{
	return lcsm::NodeT::NODE_UNKNOWN;
}

void lcsm::CGNode::swap(lcsm::CGNode &other) noexcept
{
	std::swap(m_target, other.m_target);
	std::swap(m_instructions, other.m_instructions);
}

bool lcsm::CGNode::isStatic() const noexcept
{
	return T() == lcsm::NodeT::NODE_STATIC;
}

bool lcsm::CGNode::isFast() const noexcept
{
	return T() == lcsm::NodeT::NODE_FAST;
}

bool lcsm::CGNode::isComposite() const noexcept
{
	return T() == lcsm::NodeT::NODE_COMPOSITE;
}

bool lcsm::CGNode::isDynamic() const noexcept
{
	return T() == lcsm::NodeT::NODE_DYNAMIC;
}

lcsm::CGStaticNode *lcsm::CGNode::asStatic() noexcept
{
	return nullptr;
}

const lcsm::CGStaticNode *lcsm::CGNode::asStatic() const noexcept
{
	return nullptr;
}

lcsm::CGFastNode *lcsm::CGNode::asFast() noexcept
{
	return nullptr;
}

const lcsm::CGFastNode *lcsm::CGNode::asFast() const noexcept
{
	return nullptr;
}

lcsm::CGCompositeNode *lcsm::CGNode::asComposite() noexcept
{
	return nullptr;
}

const lcsm::CGCompositeNode *lcsm::CGNode::asComposite() const noexcept
{
	return nullptr;
}

lcsm::CGDynamicNode *lcsm::CGNode::asDynamic() noexcept
{
	return nullptr;
}

const lcsm::CGDynamicNode *lcsm::CGNode::asDynamic() const noexcept
{
	return nullptr;
}

lcsm::CGObjectView &lcsm::CGNode::object() noexcept
{
	return m_target;
}

const lcsm::CGObjectView &lcsm::CGNode::object() const noexcept
{
	return m_target;
}

void lcsm::CGNode::setObject(const lcsm::CGObjectView &object) noexcept
{
	m_target = object;
}

void lcsm::CGNode::setObject(lcsm::CGObjectView &&object) noexcept
{
	m_target = std::move(object);
}

void lcsm::CGNode::setObject(CGObject *object) noexcept
{
	m_target = { object };
}

std::vector< std::pair< lcsm::CGEdge, lcsm::CGNodeView > > &lcsm::CGNode::instructions() noexcept
{
	return m_instructions;
}

const std::vector< std::pair< lcsm::CGEdge, lcsm::CGNodeView > > &lcsm::CGNode::instructions() const noexcept
{
	return m_instructions;
}

void lcsm::CGNode::addInstruction(lcsm::CGEdge &&I, lcsm::CGNode *N)
{
	view_type view = N;
	addInstruction(std::move(I), std::move(view));
}

void lcsm::CGNode::addInstruction(lcsm::CGEdge &&I, const lcsm::CGNode::view_type &N)
{
	m_instructions.emplace_back(std::move(I), N);
}

void lcsm::CGNode::addInstruction(lcsm::CGEdge &&I, lcsm::CGNode::view_type &&N)
{
	m_instructions.emplace_back(std::move(I), std::move(N));
}
*/
