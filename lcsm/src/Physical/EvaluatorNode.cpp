#include <lcsm/LCSM.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Support/PointerView.hpp>

lcsm::EvaluatorNode::EvaluatorNode() noexcept : m_objectType(lcsm::ObjectType::UnknownObjectType) {}

lcsm::ObjectType lcsm::EvaluatorNode::objectType() const noexcept
{
	return m_objectType;
}

void lcsm::EvaluatorNode::setObjectType(lcsm::ObjectType objectType) noexcept
{
	m_objectType = objectType;
}
