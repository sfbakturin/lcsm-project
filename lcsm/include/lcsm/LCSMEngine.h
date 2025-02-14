#ifndef LCSM_LCSMENGINE_H
#define LCSM_LCSMENGINE_H

#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <deque>
#include <memory>

namespace lcsm
{
	class LCSMEngine
	{
	  public:
		static LCSMEngine fromCircuit(const LCSMCircuit &circuit);
		LCSMState fork();

	  private:
		LCSMEngine() = default;

		std::unordered_map< Identifier, std::shared_ptr< EvaluatorNode > > m_objects;

	  private:
		friend class LCSMState;
		support::PointerView< EvaluatorNode > registered(Identifier id) const noexcept;
		support::PointerView< EvaluatorNode > registeredWire(Identifier id);

		void buildCircuit(std::deque< support::PointerView< const Circuit > > &bfsVisit);
		void buildCircuit(const support::PointerView< const Circuit > &circuit,
						  std::deque< support::PointerView< const Circuit > > &bfsVisit,
						  std::unordered_set< Identifier > &visited);
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMENGINE_H */
