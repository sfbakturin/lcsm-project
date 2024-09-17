#include <sim/IR/CG.h>

#include <utility>

sim::CGStaticNode::CGStaticNode(const sim::CGObjectView &object) :
	CGNode(object)
{
}

sim::CGStaticNode::CGStaticNode(sim::CGObjectView &&object) :
	CGNode(std::move(object))
{
}

sim::CGStaticNode::CGStaticNode(sim::CGObject *object) : CGNode(object) {}

sim::NodeT sim::CGStaticNode::T() const noexcept
{
	return sim::NodeT::NODE_STATIC;
}

sim::CGStaticNode *sim::CGStaticNode::asStatic() noexcept
{
	return this;
}

const sim::CGStaticNode *sim::CGStaticNode::asStatic() const noexcept
{
	return this;
}
