#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>

#include <utility>

lcsm::CGCompositeNode::CGCompositeNode(const lcsm::CGObjectView &object) : lcsm::CGNode(object) {}

lcsm::CGCompositeNode::CGCompositeNode(lcsm::CGObjectView &&object) : lcsm::CGNode(std::move(object)) {}

lcsm::CGCompositeNode::CGCompositeNode(lcsm::CGObject *object) : lcsm::CGNode(object) {}

lcsm::NodeT lcsm::CGCompositeNode::T() const noexcept
{
	return lcsm::NodeT::NODE_COMPOSITE;
}

lcsm::CGCompositeNode *lcsm::CGCompositeNode::asComposite() noexcept
{
	return this;
}

const lcsm::CGCompositeNode *lcsm::CGCompositeNode::asComposite() const noexcept
{
	return this;
}
