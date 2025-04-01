#ifndef LCSM_LCSMENGINE_H
#define LCSM_LCSMENGINE_H

#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>
#include <unordered_map>
#include <unordered_set>

#include <deque>
#include <memory>
#include <vector>

namespace lcsm
{
	class LCSM_API LCSMEngine
	{
	  public:
		static LCSMEngine fromCircuit(const LCSMCircuit &circuit);
		LCSMState fork();

		const std::vector< Identifier > &getInputIndexes() const noexcept;
		const std::vector< Identifier > &getOutputIndexes() const noexcept;
		const std::vector< Identifier > &getRootIndexes() const noexcept;

	  private:
		LCSMEngine() = default;

		std::unordered_map< Identifier, std::shared_ptr< EvaluatorNode > > m_objects;

		std::unordered_map< Identifier, std::shared_ptr< EvaluatorNode > > m_realInputs;
		std::unordered_map< Identifier, std::shared_ptr< EvaluatorNode > > m_realOutputs;
		std::unordered_map< Identifier, std::shared_ptr< EvaluatorNode > > m_realRoots;

		std::vector< Identifier > m_realInputsIds;
		std::vector< Identifier > m_realOutputsIds;
		std::vector< Identifier > m_realRootsIds;

	  private:
		friend class LCSMState;

		support::PointerView< EvaluatorNode > registered(Identifier id) const noexcept;
		support::PointerView< EvaluatorNode > registeredWire(Identifier id);
		support::PointerView< EvaluatorNode > registeredTunnel(Identifier id);

		void buildCircuit(std::deque< support::PointerView< const Component > > &queue);
		void buildCircuit(const support::PointerView< const Component > &circuit,
						  std::deque< support::PointerView< const Component > > &queue,
						  std::unordered_set< Identifier > &visited);
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMENGINE_H */
