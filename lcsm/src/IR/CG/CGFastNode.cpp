#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>

#include <utility>

lcsm::CGFastNode::CGFastNode(const lcsm::CGObjectView &object) : CGNode(object) {}

lcsm::CGFastNode::CGFastNode(lcsm::CGObjectView &&object) : CGNode(std::move(object)) {}

lcsm::CGFastNode::CGFastNode(lcsm::CGObject *object) : CGNode(object) {}

lcsm::CGNodeType lcsm::CGFastNode::nodeType() const noexcept
{
	return lcsm::CGNodeType::Fast;
}

lcsm::CGFastNode *lcsm::CGFastNode::asFast() noexcept
{
	return this;
}

const lcsm::CGFastNode *lcsm::CGFastNode::asFast() const noexcept
{
	return this;
}
