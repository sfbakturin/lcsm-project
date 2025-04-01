#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

using TestData = std::pair< DataBits, DataBits >;

static LCSMCircuit getCircuit1()
{
	LCSMCircuit circuit("Circuit0");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "Circuit0 Input");
	model::Pin *output = circuit.createPin(true, "Circuit0 Output");

	// Make connections.
	circuit.connect(input, output);

	return circuit;
}

static LCSMCircuit generator()
{
	LCSMCircuit circuit("CircuitInCircuitTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "Input");
	model::Pin *output = circuit.createPin(true, "Output");

	// Add circuit to circuit.
	LCSMCircuitView view = circuit.addCircuit(getCircuit1());
	lcsm::Component *input0 = view.find("Circuit0 Input");
	lcsm::Component *output0 = view.find("Circuit0 Output");

	// Make connections.
	circuit.connect(input, input0, model::Pin::Port::External);
	circuit.connect(output0, model::Pin::Port::External, output);

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	LCSMCircuitView view = circuit.findCircuit("Circuit0");
	Component *input = circuit.find("Input");
	Component *output = circuit.find("Output");
	Component *input0 = view.find("Circuit0 Input");
	Component *output0 = view.find("Circuit0 Output");

	// Check element's types.
	assertTrue(view.present());
	assertType(input, ComponentType::Pin);
	assertType(output, ComponentType::Pin);
	assertType(input0, ComponentType::Pin);
	assertType(output0, ComponentType::Pin);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	LCSMCircuitView view = circuit.findCircuit("Circuit0");
	Component *input = circuit.find("Input");
	Component *output = circuit.find("Output");
	Component *input0 = view.find("Circuit0 Input");
	Component *output0 = view.find("Circuit0 Output");

	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	const Identifier input0Id = input0->id();
	const Identifier output0Id = output0->id();

	// Inputs and expected: at 0 - `input`'s, at 1 - `output`'s.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { T }, { T } },
		{ { F }, { F } },
	};
	// clang-format on

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (const TestData &testdata : testdatas)
	{
		// Testing data.
		const DataBits &in = testdata.first;
		const DataBits &ref = testdata.second;

		// Put value.
		state.putValue(inputId, in);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &inputActual = state.valueOf(inputId);
		const DataBits &outputActual = state.valueOf(outputId);
		const DataBits &input0Actual = state.valueOf(input0Id);
		const DataBits &output0Actual = state.valueOf(output0Id);
		assertEquals(inputActual, ref);
		assertEquals(outputActual, ref);
		assertEquals(input0Actual, ref);
		assertEquals(output0Actual, ref);

		// Printout.
		std::cout << "<input = " << inputActual << "> --> <input0 = " << input0Actual
				  << "> --> <output0 = " << output0Actual << "> --> <output = " << outputActual << ">\n";
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
