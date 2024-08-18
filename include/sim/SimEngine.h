#ifndef SIM_SIMENGINE_H
#define SIM_SIMENGINE_H

#include <initializer_list>
#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>
#include <sim/SimContext.h>
#include <sim/Support/PointerView.hpp>
#include <unordered_map>

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

		void buildCircuit(
			const Wire &root,
			CGObject *targetRoot,
			const WiringComponent *parent,
			CGNode *parentNode,
			std::unordered_map< unsigned, bool > &visited);

		unsigned m_circuits;
		std::unordered_map< unsigned, std::shared_ptr< CGObject > > m_objects;
		std::vector< CG > m_inputs;
	};
}	 // namespace sim

#endif /* SIM_SIMENGINE_H */
