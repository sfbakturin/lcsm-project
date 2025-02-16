#include <lcsm/LCSM.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>
#include <vector>

int main()
{
	// Main circuit.
	lcsm::LCSMCircuit circuit;

	// Create input and output pins.
	lcsm::model::Pin *i0 = circuit.createPin(false);
	lcsm::model::Pin *o0 = circuit.createPin(true);

	/* Connect pins. */
	circuit.connect(i0, lcsm::model::Pin::Port::Internal, o0, lcsm::model::Pin::Port::Internal);

	const std::vector< lcsm::DataBits > values = {
		{ lcsm::Width::Bit1, lcsm::verilog::Bit::False },
		{ lcsm::Width::Bit1, lcsm::verilog::Bit::True }
	};

	/* Build runtime calculation graph from circuit. */
	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(circuit);

	/* Fork this state. */
	lcsm::LCSMState state = engine.fork();

	for (const lcsm::DataBits &value : values)
	{
		/* Put value to in0 (input pin). */
		state.putValue(i0->id(), value);

		/* Step once. */
		state.tick();

		/* Extract value from out0 (output pin). */
		const lcsm::DataBits &out = state.valueOf(o0->id());

		/* Print. */
		std::cout << "in" << i0->id() << " = " << value << ", "
				  << "out" << o0->id() << " = " << out << '\n';
	}
}
