#include <lcsm/LCSM.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Support/PointerView.hpp>

lcsm::EvaluatorNode::EvaluatorNode(lcsm::ObjectType objectType) noexcept : m_objectType(objectType) {}

lcsm::ObjectType lcsm::EvaluatorNode::objectType() const noexcept
{
	return m_objectType;
}
