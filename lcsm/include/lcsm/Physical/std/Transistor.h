#ifndef LCSM_PHYSICAL_STD_TRANSISTOR_H
#define LCSM_PHYSICAL_STD_TRANSISTOR_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>

namespace lcsm
{
	namespace physical
	{
		class Transistor : public EvaluatorNode
		{
		  public:
			Transistor(object_type_t objectType, model::Transistor::Type type);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;

			virtual void addInstant(const Instruction &instruction) override final;
			virtual void addInstant(Instruction &&instruction) override final;

			virtual std::vector< Event > invokeInstants(const Timestamp &now) override final;

			void connectBase(const support::PointerView< EvaluatorNode > &node);
			void connectSrcA(const support::PointerView< EvaluatorNode > &node);
			void connectSrcB(const support::PointerView< EvaluatorNode > &node);

		  private:
			model::Transistor::Type m_type;
			std::deque< Instruction > m_instantsBase;
			std::deque< Instruction > m_instantsSrcA;
			std::deque< Instruction > m_instantsSrcB;
			support::PointerView< Context > m_context;
			support::PointerView< EvaluatorNode > m_base;
			support::PointerView< EvaluatorNode > m_srca;
			support::PointerView< EvaluatorNode > m_srcb;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_TRANSISTOR_H */
