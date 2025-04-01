#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <iostream>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

static LCSMCircuit generator()
{
	LCSMCircuit circuit("PinTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");

	// Make connections.
	model::Wire *wire = circuit.connect(input, output);
	wire->setName("wire");

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *input = circuit.find("input");
	Component *output = circuit.find("output");
	Component *wire = circuit.find("wire");

	// Check element's types.
	assertType(input, ComponentType::Pin);
	assertType(output, ComponentType::Pin);
	assertType(wire, ComponentType::Wire);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *input = circuit.find("input");
	Component *output = circuit.find("output");
	Component *wire = circuit.find("wire");

	// Testing data.
	const std::vector< DataBits > testdatas = { { T }, { F } };

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	const Identifier wireId = wire->id();

	// Test!
	for (const DataBits &testdata : testdatas)
	{
		// Put value.
		state.putValue(inputId, testdata);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &inputActual = state.valueOf(inputId);
		const DataBits &outputActual = state.valueOf(outputId);
		const DataBits &wireActual = state.valueOf(wireId);
		assertEquals(inputActual, testdata);
		assertEquals(outputActual, testdata);
		assertEquals(wireActual, testdata);

		// Printout.
		std::cout << "<pin = " << inputActual << "> --> <wire = " << wireActual << "> --> <pin = " << outputActual << ">\n";
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
