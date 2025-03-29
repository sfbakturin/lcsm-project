#ifndef LCSM_PHYSICAL_STD_WIRE_H
#define LCSM_PHYSICAL_STD_WIRE_H

#include <lcsm/LCSM.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>

#include <deque>
#include <vector>

namespace lcsm
{
	namespace physical
	{
		class LCSM_API Wire : public EvaluatorNode
		{
		  public:
			Wire(object_type_t objectType);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;
			virtual void verifyContext() override final;

			virtual void add(Instruction &&instruction) override final;

			virtual std::vector< Event > invoke(const Timestamp &now) override final;

			void connect(const support::PointerView< EvaluatorNode > &child);

		  private:
			std::deque< Instruction > m_instants;
			std::deque< Instruction > m_pollutes;
			support::PointerView< Context > m_context;
			std::vector< support::PointerView< EvaluatorNode > > m_children;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_WIRE_H */
