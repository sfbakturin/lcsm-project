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

int main()
{
	// Main circuit.
	lcsm::LCSMCircuit circuit;

	// Create input and output pins.
	lcsm::model::Pin *i0 = circuit.createPin(false);
	lcsm::model::Pin *i1 = circuit.createPin(false);
	lcsm::model::Pin *o0 = circuit.createPin(true);
	lcsm::model::Pin *o1 = circuit.createPin(true);

	/* Connect pins. */
	lcsm::model::Wire *w0 = circuit.connect(i0, lcsm::model::Pin::Port::Internal, i1, lcsm::model::Pin::Port::Internal);
	lcsm::model::Wire *w1 = circuit.connect(w0, lcsm::model::Wire::Port::Wiring, o0, lcsm::model::Pin::Port::Internal);
	lcsm::model::Wire *w2 = circuit.connect(w0, lcsm::model::Wire::Port::Wiring, o1, lcsm::model::Pin::Port::Internal);

	const std::vector< lcsm::DataBits > values = {
		{ lcsm::Width::Bit1, lcsm::verilog::Bit::False },
		{ lcsm::Width::Bit1, lcsm::verilog::Bit::True }
	};

	/* Make a copy. */
	lcsm::LCSMCircuit copyCircuit = circuit.copy();

	/* Build runtime calculation graph from circuit. */
	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(copyCircuit);

	/* Fork this state. */
	lcsm::LCSMState state = engine.fork();

	for (const lcsm::DataBits &value0 : values)
	{
		for (const lcsm::DataBits &value1 : values)
		{
			/* Put value to in0 (input pin). */
			state.putValue(i0->id(), value0);
			state.putValue(i1->id(), value1);

			/* Step once. */
			state.tick();

			/* Extract values from pins. */
			const lcsm::DataBits &in0 = state.valueOf(i0->id());
			const lcsm::DataBits &in1 = state.valueOf(i1->id());
			const lcsm::DataBits &out0 = state.valueOf(o0->id());
			const lcsm::DataBits &out1 = state.valueOf(o1->id());
			const lcsm::DataBits &wire0 = state.valueOf(w0->id());
			const lcsm::DataBits &wire1 = state.valueOf(w1->id());
			const lcsm::DataBits &wire2 = state.valueOf(w2->id());

			/* Print. */
			std::cout << "i0 = " << in0 << ", i1 = " << in1 << ", o0 = " << out0 << ", o1 = " << out1 << "         "
					  << "w0 = " << wire0 << ", w1 = " << wire1 << ", w2 = " << wire2 << '\n';
		}
	}
}
