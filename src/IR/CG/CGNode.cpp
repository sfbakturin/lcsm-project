#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/Support/PointerView.hpp>

#include <memory>
#include <utility>
#include <vector>

sim::CGNode::CGNode(const sim::CGObjectView &object) : m_target(object) {}

sim::CGNode::CGNode(sim::CGObjectView &&object) : m_target(std::move(object)) {}

sim::CGNode::CGNode(sim::CGObject *object) : m_target(object) {}

sim::CGNode::CGNode(sim::CGNode &&other) noexcept :
	m_target(std::move(other.m_target)), m_instructions(std::move(other.m_instructions))
{
}

sim::CGNode &sim::CGNode::operator=(sim::CGNode &&other) noexcept
{
	if (this != std::addressof(other))
		sim::CGNode(std::move(other)).swap(*this);
	return *this;
}

sim::NodeT sim::CGNode::T() const noexcept
{
	return sim::NodeT::NODE_UNKNOWN;
}

void sim::CGNode::swap(sim::CGNode &other) noexcept
{
	std::swap(m_target, other.m_target);
	std::swap(m_instructions, other.m_instructions);
}

bool sim::CGNode::isStatic() const noexcept
{
	return T() == sim::NodeT::NODE_STATIC;
}

bool sim::CGNode::isFast() const noexcept
{
	return T() == sim::NodeT::NODE_FAST;
}

bool sim::CGNode::isComposite() const noexcept
{
	return T() == sim::NodeT::NODE_COMPOSITE;
}

bool sim::CGNode::isDynamic() const noexcept
{
	return T() == sim::NodeT::NODE_DYNAMIC;
}

sim::CGStaticNode *sim::CGNode::asStatic() noexcept
{
	return nullptr;
}

const sim::CGStaticNode *sim::CGNode::asStatic() const noexcept
{
	return nullptr;
}

sim::CGFastNode *sim::CGNode::asFast() noexcept
{
	return nullptr;
}

const sim::CGFastNode *sim::CGNode::asFast() const noexcept
{
	return nullptr;
}

sim::CGCompositeNode *sim::CGNode::asComposite() noexcept
{
	return nullptr;
}

const sim::CGCompositeNode *sim::CGNode::asComposite() const noexcept
{
	return nullptr;
}

sim::CGDynamicNode *sim::CGNode::asDynamic() noexcept
{
	return nullptr;
}

const sim::CGDynamicNode *sim::CGNode::asDynamic() const noexcept
{
	return nullptr;
}

sim::CGObjectView &sim::CGNode::object() noexcept
{
	return m_target;
}

const sim::CGObjectView &sim::CGNode::object() const noexcept
{
	return m_target;
}

void sim::CGNode::setObject(const sim::CGObjectView &object) noexcept
{
	m_target = object;
}

void sim::CGNode::setObject(sim::CGObjectView &&object) noexcept
{
	m_target = std::move(object);
}

void sim::CGNode::setObject(CGObject *object) noexcept
{
	m_target = { object };
}

std::vector< std::pair< sim::CGEdge, sim::CGNodeView > > &sim::CGNode::instructions() noexcept
{
	return m_instructions;
}

const std::vector< std::pair< sim::CGEdge, sim::CGNodeView > > &
	sim::CGNode::instructions() const noexcept
{
	return m_instructions;
}

void sim::CGNode::addInstruction(sim::CGEdge &&I, sim::CGNode *N)
{
	view_type view = N;
	addInstruction(std::move(I), std::move(view));
}

void sim::CGNode::addInstruction(sim::CGEdge &&I, const sim::CGNode::view_type &N)
{
	m_instructions.emplace_back(std::move(I), N);
}

void sim::CGNode::addInstruction(sim::CGEdge &&I, sim::CGNode::view_type &&N)
{
	m_instructions.emplace_back(std::move(I), std::move(N));
}
