#include <lcsm/LCSM.h>
#include <lcsm/LCSMBuilder.h>
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
	lcsm::LCSMCircuit circuit;
	lcsm::LCSMBuilder builder(circuit);

	/* Create input and output pins. */
	lcsm::model::Pin *in0 = builder.CreatePin(false);
	lcsm::model::Pin *out0 = builder.CreatePin(true);

	/* Connect pins. */
	builder.ConnectInternally(in0, lcsm::model::Pin::Port::Internal, out0, lcsm::model::Pin::Port::Internal);

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
		state.putValue(in0->id(), value);

		/* Step once. */
		state.stepOnce();

		/* Extract value from out0 (output pin). */
		const lcsm::DataBits &out = state.valueOf(out0->id());

		/* Print. */
		std::cout << "in" << in0->id() << " = " << value << ", "
				  << "out" << out0->id() << " = " << out << '\n';
	}
}
