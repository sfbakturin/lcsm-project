#ifndef LCSM_PHYSICAL_STD_BUTTON_H
#define LCSM_PHYSICAL_STD_BUTTON_H

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
		class LCSM_API Button : public EvaluatorNode
		{
		  public:
			Button(object_type_t objectType, bool activeOnPress) noexcept;

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;
			virtual void verifyContext() override final;

			virtual void add(Instruction &&instruction) override final;

			virtual void invoke(const Timestamp &now, std::deque< Event > &events) override final;

			void connect(const support::PointerView< EvaluatorNode > &node) noexcept;

		  private:
			bool m_activeOnPress;
			bool m_wasPolluted;
			support::PointerView< Context > m_context;
			support::PointerView< EvaluatorNode > m_wire;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_BUTTON_H */
