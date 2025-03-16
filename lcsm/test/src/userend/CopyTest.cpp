#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>
#include <vector>

using namespace lcsm;

int main()
{
	// Main circuit.
	LCSMCircuit circuit("CopyTest");

	// Create input and output pins.
	model::Pin *i0 = circuit.createPin(false);
	model::Pin *o0 = circuit.createPin(true);

	// Connect pins.
	circuit.connect(i0, o0);

	// Make a copy of main circuit.
	LCSMCircuit copyCircuit = circuit.copy();

	// Find those input and output.
	Circuit *input = circuit.find(i0->id());
	Circuit *output = circuit.find(o0->id());

	// Values to put to pin.
	const std::vector< lcsm::DataBits > values = {
		{ lcsm::Width::Bit1, lcsm::verilog::Bit::False },
		{ lcsm::Width::Bit1, lcsm::verilog::Bit::True }
	};

	// Build runtime calculation graph from circuit.
	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(copyCircuit);

	// Fork this state.
	lcsm::LCSMState state = engine.fork();

	// Run!
	for (const lcsm::DataBits &value : values)
	{
		// Put value to input.
		state.putValue(input->id(), value);

		// Step once.
		state.tick();

		// Extract value from output.
		const lcsm::DataBits &out = state.valueOf(output->id());

		// Print.
		std::cout << "in" << i0->id() << " = " << value << ", "
				  << "out" << o0->id() << " = " << out << '\n';
	}
}
