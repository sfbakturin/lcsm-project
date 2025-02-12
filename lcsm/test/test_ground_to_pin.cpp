#include <lcsm/LCSM.h>
#include <lcsm/LCSMBuilder.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>

int main()
{
	lcsm::LCSMCircuit circuit;
	lcsm::LCSMBuilder builder(circuit);

	/* Create input and output pins. */
	lcsm::model::Ground *ground = builder.CreateGround(lcsm::Width::Byte);
	lcsm::model::Pin *out = builder.CreatePin(true);

	/* Connect pins. */
	builder.Connect(ground, lcsm::model::Ground::Port::Wiring, out, lcsm::model::Pin::Port::Internal);

	/* Build runtime calculation graph from circuit. */
	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(circuit);

	/* Fork this state. */
	lcsm::LCSMState state = engine.fork();

	/* Step once. */
	state.tick();

	/* Extract value from output Pin. */
	const lcsm::DataBits &outValue = state.valueOf(out->id());

	/* Print. */
	std::cout << "out = " << outValue << '\n';
}
