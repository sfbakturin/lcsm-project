#ifndef LCSM_PHYSICAL_STD_WIRE_H
#define LCSM_PHYSICAL_STD_WIRE_H

#include <lcsm/LCSM.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>
#include <vector>

namespace lcsm
{
	namespace physical
	{
		class Wire : public EvaluatorNode
		{
		  public:
			Wire(object_type_t objectType);

			virtual const DataBits &read() const override final;

			virtual Width width() const override final;
			virtual bool checkWidth(const DataBits &value) const override final;

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;

			virtual void addInstant(const Instruction &instruction) override final;
			virtual void addInstant(Instruction &&instruction) override final;
			virtual std::vector< Event > invokeInstants(const Timestamp &now) override final;

			void connect(const support::PointerView< EvaluatorNode > &child);

		  private:
			std::deque< Instruction > m_instants;
			support::PointerView< Context > m_context;
			std::vector< support::PointerView< EvaluatorNode > > m_children;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_WIRE_H */
