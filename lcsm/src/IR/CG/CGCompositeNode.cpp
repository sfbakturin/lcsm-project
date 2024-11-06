#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>

#include <utility>

lcsm::CGCompositeNode::CGCompositeNode(const lcsm::CGObjectView &object) : lcsm::CGNode(object) {}

lcsm::CGCompositeNode::CGCompositeNode(lcsm::CGObjectView &&object) : lcsm::CGNode(std::move(object)) {}

lcsm::CGCompositeNode::CGCompositeNode(lcsm::CGObject *object) : lcsm::CGNode(object) {}

lcsm::CGNodeType lcsm::CGCompositeNode::nodeType() const noexcept
{
	return lcsm::CGNodeType::Composite;
}

lcsm::CGCompositeNode *lcsm::CGCompositeNode::asComposite() noexcept
{
	return this;
}

const lcsm::CGCompositeNode *lcsm::CGCompositeNode::asComposite() const noexcept
{
	return this;
}
