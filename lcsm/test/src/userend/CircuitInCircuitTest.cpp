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

static LCSMCircuit getCircuit()
{
	LCSMCircuit circuit("CircuitInCircuitTest");

	model::Pin *input = circuit.createPin(false, "circ0_input");
	model::Pin *output = circuit.createPin(true, "circ0_output");

	circuit.connect(input, model::Pin::Port::Internal, output, model::Pin::Port::Internal);

	return circuit;
}

int main()
{
	LCSMCircuit circuit("CircuitTest");

	model::Pin *input = circuit.createPin(false, "circ1_input");
	model::Pin *output = circuit.createPin(true, "circ1_output");

	LCSMCircuitView view = circuit.addCircuit(getCircuit());

	lcsm::Circuit *input0 = view.find("circ0_input");
	lcsm::Circuit *output0 = view.find("circ0_output");

	circuit.connect(input, model::Pin::Port::Internal, input0, model::Pin::Port::External);
	circuit.connect(output0, model::Pin::Port::External, output, model::Pin::Port::Internal);

	std::cout << "Dumping '" << circuit.name() << "'...\n";
	std::cout << circuit.dumpToString();
	std::cout << "... end of dumping\n";

	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	LCSMState state = engine.fork();

	const std::vector< DataBits > values = { { Width::Bit1, verilog::Bit::False }, { Width::Bit1, verilog::Bit::True } };

	for (const DataBits &value : values)
	{
		state.putValue(input->id(), value);
		state.tick();
		std::cout << "in = " << value << " =>  out = " << state.valueOf(output0->id()) << '\n';
	}
}
