#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>

#include <utility>

sim::CGFastNode::CGFastNode(const sim::CGObjectView &object) : CGNode(object) {}

sim::CGFastNode::CGFastNode(sim::CGObjectView &&object) :
	CGNode(std::move(object))
{
}

sim::CGFastNode::CGFastNode(sim::CGObject *object) : CGNode(object) {}

sim::NodeT sim::CGFastNode::T() const noexcept
{
	return sim::NodeT::NODE_FAST;
}

sim::CGFastNode *sim::CGFastNode::asFast() noexcept
{
	return this;
}

const sim::CGFastNode *sim::CGFastNode::asFast() const noexcept
{
	return this;
}
