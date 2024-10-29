#include <lcsm/IR/CG.h>

#include <utility>

lcsm::CGDynamicNode::CGDynamicNode(const lcsm::CGObjectView &object) : CGNode(object) {}

lcsm::CGDynamicNode::CGDynamicNode(lcsm::CGObjectView &&object) : CGNode(std::move(object)) {}

lcsm::CGDynamicNode::CGDynamicNode(lcsm::CGObject *object) : CGNode(object) {}

lcsm::NodeT lcsm::CGDynamicNode::T() const noexcept
{
	return lcsm::NodeT::NODE_DYNAMIC;
}

lcsm::CGDynamicNode *lcsm::CGDynamicNode::asDynamic() noexcept
{
	return this;
}

const lcsm::CGDynamicNode *lcsm::CGDynamicNode::asDynamic() const noexcept
{
	return this;
}
