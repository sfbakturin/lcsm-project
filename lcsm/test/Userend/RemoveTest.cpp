#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>

int main()
{
	// Main circuit.
	lcsm::LCSMCircuit circuit;

	// Create input and output pins.
	lcsm::model::Pin *i0 = circuit.createPin(false);
	lcsm::model::Pin *o0 = circuit.createPin(true);

	circuit.connect(i0, lcsm::model::Pin::Port::Internal, o0, lcsm::model::Pin::Internal);

	// Delete input and output.
	circuit.remove(o0);

	// Start it.
	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(circuit);
	lcsm::LCSMState state = engine.fork();

	const lcsm::DataBits value(lcsm::Width::QuadWord, lcsm::verilog::Bit::True);
	state.putValue(i0->id(), value);

	state.tick();

	std::cout << "Value of only Pin:" << state.valueOf(i0->id()) << '\n';
}
