#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Verilog/Bit.h>

int main()
{
	lcsm::LCSMCircuit circuit;

	// Create input and output pins.
	lcsm::model::Pin *i0 = circuit.createPin(false);
	lcsm::model::Pin *i1 = circuit.createPin(false);

	/* Connect pins. */
	circuit.connect(i0, lcsm::model::Pin::Port::Internal, i1, lcsm::model::Pin::Port::Internal);

	/* Build runtime calculation graph from circuit. */
	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(circuit);

	/* Fork this state. */
	lcsm::LCSMState state = engine.fork();

	/* Put True/False to inputs. */
	state.putValue(i0->id(), { lcsm::Width::Bit1, lcsm::verilog::Bit::False });
	state.putValue(i1->id(), { lcsm::Width::Bit1, lcsm::verilog::Bit::True });

	/* Step once. */
	state.tick();

	/* Put True/False to inputs. */
	state.putValue(i0->id(), { lcsm::Width::Bit7, lcsm::verilog::Bit::True });
	state.putValue(i1->id(), { lcsm::Width::Bit7, lcsm::verilog::Bit::False });

	/* Step once. */
	state.tick();
}
