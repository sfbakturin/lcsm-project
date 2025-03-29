#ifndef LCSM_PHYSICAL_STD_TRANSMISSIONGATE_H
#define LCSM_PHYSICAL_STD_TRANSMISSIONGATE_H

#include <lcsm/LCSM.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>

#include <deque>

namespace lcsm
{
	namespace physical
	{
		class LCSM_API TransmissionGate : public EvaluatorNode
		{
		  public:
			TransmissionGate(object_type_t objectType);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;
			virtual void verifyContext() override final;

			virtual void add(Instruction &&instruction) override final;

			virtual std::vector< Event > invoke(const Timestamp &now) override final;

			void connectBase(const support::PointerView< EvaluatorNode > &node);
			void connectSrcA(const support::PointerView< EvaluatorNode > &node);
			void connectSrcB(const support::PointerView< EvaluatorNode > &node);
			void connectSrcC(const support::PointerView< EvaluatorNode > &node);

		  private:
			std::deque< Instruction > m_instantsBase;
			std::deque< Instruction > m_instantsSrcA;
			std::deque< Instruction > m_instantsSrcB;
			std::deque< Instruction > m_instantsSrcC;
			support::PointerView< Context > m_context;
			support::PointerView< EvaluatorNode > m_base;
			support::PointerView< EvaluatorNode > m_srca;
			support::PointerView< EvaluatorNode > m_srcb;
			support::PointerView< EvaluatorNode > m_srcc;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_TRANSMISSIONGATE_H */
