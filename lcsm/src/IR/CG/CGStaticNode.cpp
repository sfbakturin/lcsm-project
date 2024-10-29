#include <lcsm/IR/CG.h>

#include <utility>

lcsm::CGStaticNode::CGStaticNode(const lcsm::CGObjectView &object) : CGNode(object) {}

lcsm::CGStaticNode::CGStaticNode(lcsm::CGObjectView &&object) : CGNode(std::move(object)) {}

lcsm::CGStaticNode::CGStaticNode(lcsm::CGObject *object) : CGNode(object) {}

lcsm::NodeT lcsm::CGStaticNode::T() const noexcept
{
	return lcsm::NodeT::NODE_STATIC;
}

lcsm::CGStaticNode *lcsm::CGStaticNode::asStatic() noexcept
{
	return this;
}

const lcsm::CGStaticNode *lcsm::CGStaticNode::asStatic() const noexcept
{
	return this;
}
