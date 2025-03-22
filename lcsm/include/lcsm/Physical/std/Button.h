#ifndef LCSM_PHYSICAL_STD_BUTTON_H
#define LCSM_PHYSICAL_STD_BUTTON_H

#include <lcsm/LCSM.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>

namespace lcsm
{
	namespace physical
	{
		class Button : public EvaluatorNode
		{
		  public:
			Button(object_type_t objectType, bool activeOnPress);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;
			virtual void verifyContext() override final;

			virtual void addInstant(const Instruction &instruction) override final;
			virtual void addInstant(Instruction &&instruction) override final;

			virtual std::vector< Event > invokeInstants(const Timestamp &now) override final;

			void connect(const support::PointerView< EvaluatorNode > &node);

		  private:
			bool m_activeOnPress;
			support::PointerView< Context > m_context;
			support::PointerView< EvaluatorNode > m_wire;
			std::deque< Instruction > m_instants;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_BUTTON_H */
