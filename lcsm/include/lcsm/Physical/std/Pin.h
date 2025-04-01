#ifndef LCSM_PHYSICAL_STD_PIN_H
#define LCSM_PHYSICAL_STD_PIN_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>

#include <deque>

namespace lcsm
{
	namespace physical
	{
		class LCSM_API Pin : public EvaluatorNode
		{
		  public:
			Pin(object_type_t objectType, bool output, Width width);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;
			virtual void verifyContext() override final;

			virtual void add(Instruction &&instruction) override final;

			virtual void invoke(const Timestamp &now, std::deque< Event > &events) override final;

			void connectInternal(const support::PointerView< EvaluatorNode > &internal) noexcept;
			void connectExternal(const support::PointerView< EvaluatorNode > &external) noexcept;

		  private:
			bool m_output;
			Width m_width;
			bool m_wasPolluted;
			std::deque< Instruction > m_instants;
			support::PointerView< Context > m_context;
			support::PointerView< EvaluatorNode > m_internalConnect;
			support::PointerView< EvaluatorNode > m_externalConnect;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_PIN_H */
