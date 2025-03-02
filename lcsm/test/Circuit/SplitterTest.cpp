#include "lcsm/Model/Width.h"
#include "lcsm/Physical/DataBits.h"
#include "lcsm/Verilog/Bit.h"
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Splitter.h>

#include <cassert>
#include <iostream>

using namespace lcsm;

static LCSMCircuit getCircuit()
{
	LCSMCircuit circuit;
	model::Pin *input = circuit.createPin(false, "input");
	model::Splitter *splitter = circuit.createSplitter("splitter");
	model::Pin *output1 = circuit.createPin(true, "output1");
	model::Pin *output2 = circuit.createPin(true, "output2");
	circuit.connect(input, splitter);
	circuit.connect(splitter, model::Splitter::Port::Out0, output1);
	circuit.connect(splitter, model::Splitter::Port::Out1, output2);
	return circuit;
}

int main()
{
	LCSMCircuit circuit = getCircuit();

	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");

	// Check if they exists.
	assert(input);
	assert(output1);
	assert(output2);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Put 2-bit (00) value to input.
	DataBits bits{ Width::Bit2, verilog::Bit::False };
	state.putValue(input->id(), bits);

	// Step once.
	state.tick();

	// Printout values on outputs.
	std::cout << "input = " << state.valueOf(input->id()) << " => output1 = " << state.valueOf(output1->id())
			  << ", output2 = " << state.valueOf(output2->id()) << '\n';
}
