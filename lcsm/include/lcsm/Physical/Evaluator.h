#ifndef LCSM_PHYSICAL_EVALUATOR_H
#define LCSM_PHYSICAL_EVALUATOR_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>

#include <cstddef>
#include <deque>

namespace lcsm
{
	class LCSM_API EvaluatorNode
	{
	  public:
		EvaluatorNode(object_type_t objectType) noexcept;
		virtual ~EvaluatorNode() noexcept = default;

		object_type_t objectType() const noexcept;

		virtual NodeType nodeType() const noexcept = 0;

		virtual std::size_t contextSize() const noexcept = 0;
		virtual std::size_t privateContextSize() const noexcept = 0;

		virtual void setContext(const support::PointerView< Context > &context) = 0;
		virtual void resetContext() noexcept = 0;
		virtual void verifyContext() = 0;

		virtual void add(Instruction &&instruction) = 0;

		std::deque< Event > invoke(const Timestamp &now);
		virtual void invoke(const Timestamp &now, std::deque< Event > &events) = 0;

	  private:
		object_type_t m_objectType;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_EVALUATOR_H */
