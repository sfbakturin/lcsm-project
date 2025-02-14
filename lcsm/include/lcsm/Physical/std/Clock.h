#ifndef LCSM_PHYSICAL_STD_CLOCK_H
#define LCSM_PHYSICAL_STD_CLOCK_H

#include <lcsm/LCSM.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/PointerView.hpp>

#include <vector>

namespace lcsm
{
	namespace physical
	{
		class Clock : public EvaluatorNode
		{
		  public:
			Clock(ObjectType objectType, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;

			virtual const DataBits &read() const override final;

			virtual Width width() const override final;
			virtual bool checkWidth(const DataBits &value) const override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;

			virtual void addInstant(const Instruction &instruction) override final;
			virtual void addInstant(Instruction &&instruction) override final;

			virtual std::vector< Event > invokeInstants(const Timestamp &now) override final;

			void connect(const support::PointerView< EvaluatorNode > &node) noexcept;

		  private:
			support::PointerView< EvaluatorNode > m_connect;
			support::PointerView< Context > m_context;
			unsigned m_highDuration;
			unsigned m_lowDuration;
			unsigned m_phaseOffset;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_CLOCK_H */
