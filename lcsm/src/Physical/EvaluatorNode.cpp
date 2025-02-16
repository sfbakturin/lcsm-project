#include <lcsm/LCSM.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Support/PointerView.hpp>

lcsm::EvaluatorNode::EvaluatorNode(lcsm::object_type_t objectType) noexcept : m_objectType(objectType) {}

lcsm::object_type_t lcsm::EvaluatorNode::objectType() const noexcept
{
	return m_objectType;
}
