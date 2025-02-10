#include <lcsm/LCSM.h>
#include <lcsm/LCSMBuilder.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>

int main()
{
	lcsm::LCSMCircuit circuit;
	lcsm::LCSMBuilder builder(circuit);

	/* Create input and output pins. */
	lcsm::model::Pin *in0 = builder.CreatePin(false);
	lcsm::model::Pin *in1 = builder.CreatePin(false);

	/* Connect pins. */
	lcsm::model::Wire *w = builder.Connect(in0, lcsm::model::Pin::Port::Internal, in1, lcsm::model::Pin::Port::Internal);

	/* Build runtime calculation graph from circuit. */
	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(circuit);

	/* Fork this state. */
	lcsm::LCSMState state = engine.fork();

	/* Put True/False to inputs. */
	state.putValue(in0->id(), { lcsm::Width::Bit1, lcsm::verilog::Bit::False });
	state.putValue(in1->id(), { lcsm::Width::Bit1, lcsm::verilog::Bit::True });

	/* Step once. */
	state.tick();

	/* Extract value from wire between pins. */
	const lcsm::DataBits &out1 = state.valueOf(w->id());

	/* Print. */
	std::cout << "w [ins 1] = " << out1 << '\n';

	/* Put True/False to inputs. */
	state.putValue(in0->id(), { lcsm::Width::Bit7, lcsm::verilog::Bit::True });
	state.putValue(in1->id(), { lcsm::Width::Bit7, lcsm::verilog::Bit::False });

	/* Step once. */
	state.tick();

	/* Extract value from wire between pins. */
	const lcsm::DataBits &out2 = state.valueOf(w->id());

	/* Print. */
	std::cout << "w [ins 2] = " << out2 << '\n';
}
