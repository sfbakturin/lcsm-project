#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>

#include <utility>

sim::CGCompositeNode::CGCompositeNode(const sim::CGObjectView &object) :
	sim::CGNode(object)
{
}

sim::CGCompositeNode::CGCompositeNode(sim::CGObjectView &&object) :
	sim::CGNode(std::move(object))
{
}

sim::CGCompositeNode::CGCompositeNode(sim::CGObject *object) :
	sim::CGNode(object)
{
}

sim::NodeT sim::CGCompositeNode::T() const noexcept
{
	return sim::NodeT::NODE_COMPOSITE;
}

sim::CGCompositeNode *sim::CGCompositeNode::asComposite() noexcept
{
	return this;
}

const sim::CGCompositeNode *sim::CGCompositeNode::asComposite() const noexcept
{
	return this;
}
