#include <lcsm/IR/DataBits.h>
#include <lcsm/LCSMBuilder.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit/Pin.h>
#include <lcsm/Model/Width.h>
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
	builder.ConnectPin(in0, out0, 0);

	const std::vector< lcsm::DataBits > values = {
		{ lcsm::model::Width::Bit1, lcsm::verilog::Bit::False },
		{ lcsm::model::Width::Bit1, lcsm::verilog::Bit::True }
	};

	/* Build runtime calculation graph from circuit. */
	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(circuit);

	for (const lcsm::DataBits &value : values)
	{
		/* Fork this state. */
		lcsm::LCSMState state = engine.fork();

		/* Put value to in0 (input pin). */
		engine.putValue(in0->ID(), value);

		/* Step once. */
		state.stepOnce();

		/* Extract value from out0 (output pin). */
		const lcsm::DataBits &out = engine.valueOf(out0->ID());

		/* Print. */
		std::cout << "in" << in0->ID() << " = " << value << ", "
				  << "out" << out0->ID() << " = " << out << '\n';
	}
}
