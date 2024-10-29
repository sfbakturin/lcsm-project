#ifndef LCSM_LCSMENGINE_H
#define LCSM_LCSMENGINE_H

#include <initializer_list>
#include <lcsm/Component/Component.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/IR/Value.h>
#include <lcsm/LCSMContext.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <deque>
#include <memory>
#include <vector>

namespace lcsm
{
	class LCSMEngine
	{
	  public:
		LCSMEngine();

		void addCircuit(LCSMCircuit &circuit);

		std::vector< Value > invokeFull(std::initializer_list< Value > I);

	  private:
		CGWire *registerWire(Identifier ID);
		CGPinInput *registerPinInput(Identifier ID);
		CGPinOutput *registerPinOutput(Identifier ID);
		CGConstant *registerConstant(Identifier ID);
		CGPower *registerPower(Identifier ID);
		CGGround *registerGround(Identifier ID);
		CGTransistorBase *registerTransistorBase(Identifier ID);
		CGTransistorInout *registerTransistorInout(Identifier ID);
		CGTransistorState *registerTransistorState(Identifier ID);

		CGWire *registeredWire(Identifier ID);
		CGPinInput *registeredPinInput(Identifier ID);
		CGPinOutput *registeredPinOutput(Identifier ID);
		CGConstant *registeredConstant(Identifier ID);
		CGPower *registeredPower(Identifier ID);
		CGGround *registeredGround(Identifier ID);
		CGTransistorBase *registeredTransistorBase(Identifier ID);
		CGTransistorInout *registeredTransistorInout(Identifier ID);
		CGTransistorState *registeredTransistorState(Identifier ID);

		CGStaticNode *registeredStaticNode(Identifier ID, CGObject *object);
		CGFastNode *registeredFastNode(Identifier ID, CGObject *object);
		CGCompositeNode *registeredCompositeNode(Identifier ID, CGObject *object);
		CGDynamicNode *registeredDynamicNode(Identifier ID, CGObject *object);

		void buildCircuitIOComp(
			std::unordered_set< lcsm::Identifier > &visited,
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

		unsigned m_circuits;
		CG m_inputs;
		std::unordered_map< Identifier, std::shared_ptr< CGObject > > m_objects;
		std::unordered_map< Identifier, std::shared_ptr< CGNode > > m_nodes;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMENGINE_H */
