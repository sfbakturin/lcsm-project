#include <sim/IR/CG.h>

#include <utility>

sim::CGDynamicNode::CGDynamicNode(const sim::CGObjectView &object) :
	CGNode(object)
{
}

sim::CGDynamicNode::CGDynamicNode(sim::CGObjectView &&object) :
	CGNode(std::move(object))
{
}

sim::CGDynamicNode::CGDynamicNode(sim::CGObject *object) : CGNode(object) {}

sim::NodeT sim::CGDynamicNode::T() const noexcept
{
	return sim::NodeT::NODE_DYNAMIC;
}

sim::CGDynamicNode *sim::CGDynamicNode::asDynamic() noexcept
{
	return this;
}

const sim::CGDynamicNode *sim::CGDynamicNode::asDynamic() const noexcept
{
	return this;
}
