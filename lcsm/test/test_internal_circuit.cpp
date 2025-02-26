#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>

#include <iostream>

static lcsm::LCSMCircuit GetDummy()
{
	lcsm::LCSMCircuit circuit;
	lcsm::model::Pin *i0 = circuit.createPin(false);
	lcsm::model::Pin *o0 = circuit.createPin(true);
	circuit.connect(i0, lcsm::model::Pin::Port::Internal, o0, lcsm::model::Pin::Port::Internal);
	return circuit;
}

int main()
{
	lcsm::LCSMCircuit circuit;

	lcsm::model::Pin *i0 = circuit.createPin(false);
	lcsm::model::Pin *o0 = circuit.createPin(true);
	const lcsm::LCSMCircuit *c0 = circuit.addCircuit(GetDummy());
	lcsm::Circuit *c0i0 = c0->inputs().begin()->second.get();
	lcsm::Circuit *c0o0 = c0->outputs().begin()->second.get();

	circuit.connect(i0, lcsm::model::Pin::Port::Internal, c0i0, lcsm::model::Pin::Port::External);
	circuit.connect(c0o0, lcsm::model::Pin::Port::External, o0, lcsm::model::Pin::Port::Internal);

	lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(circuit);

	lcsm::LCSMState state = engine.fork();

	const std::vector< lcsm::DataBits > values = {
		{ lcsm::Width::Bit1, lcsm::verilog::Bit::False },
		{ lcsm::Width::Bit1, lcsm::verilog::Bit::True }
	};

	for (const lcsm::DataBits &value : values)
	{
		state.putValue(i0->id(), value);
		state.tick();
		const lcsm::DataBits &out = state.valueOf(c0o0->id());
		std::cout << "in = " << value << " =>  out = " << out << '\n';
	}
}
