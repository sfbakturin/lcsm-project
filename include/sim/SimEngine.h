#ifndef SIM_SIMENGINE_H
#define SIM_SIMENGINE_H

#include <initializer_list>
#include <sim/Component/Component.h>
#include <sim/Component/Identifier.h>
#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>
#include <sim/SimContext.h>
#include <sim/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <deque>
#include <memory>
#include <vector>

namespace sim
{
	class SimEngine
	{
	  public:
		SimEngine();

		void addCircuit(SimCircuit &circuit);

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
			std::unordered_set< sim::Identifier > &visited,
			std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
			const sim::IOComponent *ioComp);
		void buildCircuitWiringComp(
			std::unordered_set< sim::Identifier > &visited,
			std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
			const sim::WiringComponent *wiringComp);
		void buildCircuitCircuitComp(
			std::unordered_set< sim::Identifier > &visited,
			std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
			const sim::CircuitComponent *circuitComp);

		void buildCircuit(std::deque< support::PointerView< const sim::Component > > &bfsVisit);

		unsigned m_circuits;
		CG m_inputs;
		std::unordered_map< Identifier, std::shared_ptr< CGObject > > m_objects;
		std::unordered_map< Identifier, std::shared_ptr< CGNode > > m_nodes;
	};
}	 // namespace sim

#endif /* SIM_SIMENGINE_H */
