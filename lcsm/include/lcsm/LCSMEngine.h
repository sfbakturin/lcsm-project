#ifndef LCSM_LCSMENGINE_H
#define LCSM_LCSMENGINE_H

#include <lcsm/Component/Component.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMState.h>
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

		const DataBits &valueOf(Identifier identifier) const;

		void putValue(Identifier identifier, const DataBits &value);
		void putValue(Identifier identifier, DataBits &&value);

		void resetValues() noexcept;

	  private:
		LCSMEngine() = default;

		CGWire *registeredWire(Identifier ID);
		CGPinInput *registeredPinInput(Identifier ID);
		CGPinOutput *registeredPinOutput(Identifier ID);
		CGConstant *registeredConstant(Identifier ID);
		CGPower *registeredPower(Identifier ID);
		CGGround *registeredGround(Identifier ID);
		CGTransistorBase *registeredTransistorBase(Identifier ID);
		CGTransistorInout *registeredTransistorInout(Identifier ID);
		CGTransistorState *registeredTransistorState(Identifier ID);

		void buildCircuitIOComp(std::unordered_set< lcsm::Identifier > &visited,
								std::deque< lcsm::support::PointerView< const lcsm::Component > > &bfsVisit,
								const lcsm::IOComponent *ioComp);
		void buildCircuitWiringComp(
			std::unordered_set< lcsm::Identifier > &visited,
			std::deque< lcsm::support::PointerView< const lcsm::Component > > &bfsVisit,
			const lcsm::WiringComponent *wiringComp);
		void buildCircuitCircuitComp(
			std::unordered_set< lcsm::Identifier > &visited,
			std::deque< lcsm::support::PointerView< const lcsm::Component > > &bfsVisit,
			const lcsm::CircuitComponent *circuitComp);

		void buildCircuit(std::deque< support::PointerView< const lcsm::Component > > &bfsVisit);

		CG m_inputs;
		std::unordered_map< Identifier, std::shared_ptr< CGObject > > m_objects;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMENGINE_H */
