#ifndef LCSM_PHYSICAL_STD_SPLITTER_H
#define LCSM_PHYSICAL_STD_SPLITTER_H

#include <lcsm/LCSM.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>

namespace lcsm
{
	namespace physical
	{
		class Splitter : public EvaluatorNode
		{
		  public:
			Splitter(object_type_t objectType);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;

			virtual void addInstant(const Instruction &instruction) override final;
			virtual void addInstant(Instruction &&instruction) override final;

			virtual std::vector< Event > invokeInstants(const Timestamp &now) override final;

			void connectInternal(const support::PointerView< EvaluatorNode > &internal);
			void connectExternal(const support::PointerView< EvaluatorNode > &external);

		  private:
			std::deque< Instruction > m_instants;
			support::PointerView< Context > m_context;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_SPLITTER_H */
