#ifndef LCSM_PHYSICAL_STD_SPLITTER_H
#define LCSM_PHYSICAL_STD_SPLITTER_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <deque>
#include <utility>
#include <vector>

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
			virtual void verifyContext() override final;

			virtual void addInstant(const Instruction &instruction) override final;
			virtual void addInstant(Instruction &&instruction) override final;

			virtual std::vector< Event > invokeInstants(const Timestamp &now) override final;

			void connectInput(const support::PointerView< EvaluatorNode > &input);
			void connectOut(const support::PointerView< EvaluatorNode > &out, const std::pair< lcsm::width_t, lcsm::width_t > &index);

		  private:
			std::deque< Instruction > m_instants;
			support::PointerView< Context > m_context;
			support::PointerView< EvaluatorNode > m_input;
			std::vector< support::PointerView< EvaluatorNode > > m_outputs;
			std::vector< std::pair< lcsm::width_t, lcsm::width_t > > m_indexes;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_SPLITTER_H */
