#ifndef SIM_SIMENGINE_H
#define SIM_SIMENGINE_H

#include <initializer_list>
#include <sim/Component/Component.h>
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
		CGPinInput *registerPinInput(unsigned ID);
		CGPinOutput *registerPinOutput(unsigned ID);
		CGWire *registerWire(unsigned ID);

		CGPinInput *registeredPinInput(unsigned ID);
		CGPinOutput *registeredPinOutput(unsigned ID);
		CGWire *registeredWire(unsigned ID);

		CGNode *registeredNode(unsigned ID, CGObject *object = nullptr);

		void buildCircuitIOComp(
			std::unordered_set< unsigned > &visited,
			std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
			const sim::IOComponent *ioComp);
		void buildCircuitWiringComp(
			std::unordered_set< unsigned > &visited,
			std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
			const sim::WiringComponent *wiringComp);
		void buildCircuitCircuitComp(
			std::unordered_set< unsigned > &visited,
			std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
			const sim::CircuitComponent *circuitComp);

		void buildCircuit(std::deque< support::PointerView< const sim::Component > > &bfsVisit);

		unsigned m_circuits;
		CG m_inputs;
		std::unordered_map< unsigned, std::shared_ptr< CGObject > > m_objects;
		std::unordered_map< unsigned, CGNode > m_nodes;
	};
}	 // namespace sim

#endif /* SIM_SIMENGINE_H */
