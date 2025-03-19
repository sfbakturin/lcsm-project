#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

using TestData = std::pair< DataBits, DataBits >;

static LCSMCircuit generator()
{
	LCSMCircuit circuit("RemoveOperationTest");

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
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *wire = circuit.find("wire");

	// Check element's types.
	assertType(input, CircuitType::Pin);
	assertType(output, CircuitType::Pin);
	assertType(wire, CircuitType::Wire);
}

static void test1_removeOutput(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *wire = circuit.find("wire");

	// Remove output.
	circuit.remove(output);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Sizes of engine.
	const std::size_t numOfInputs = engine.getInputIndexes().size();
	const std::size_t numOfOutputs = engine.getOutputIndexes().size();
	const std::size_t numOfRoots = engine.getRootIndexes().size();
	assertEquals(numOfInputs, 1);
	assertEquals(numOfOutputs, 0);
	assertEquals(numOfRoots, 0);

	// Fork new state.
	LCSMState state = engine.fork();

	// Inputs and expected: at 0 - Pin's, at 1 - Wire's.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { T }, { T } },
		{ { F }, { F } },
	};
	// clang-format on

	// Test!
	for (const TestData &testdata : testdatas)
	{
		// Test's data.
		const DataBits &expectedInput = testdata.first;
		const DataBits &expectedWire = testdata.second;

		// Put value to input.
		state.putValue(input->id(), expectedInput);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &actualInput = state.valueOf(input->id());
		const DataBits &actualWire = state.valueOf(wire->id());
		assertEquals(actualInput, expectedInput);
		assertEquals(actualWire, expectedWire);

		// Printout.
		std::cout << "<pin = " << actualInput << "> --> <wire = " << actualWire << ">\n";
	}
}

void test2_removeInput(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");

	// Remove input.
	circuit.remove(input);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Sizes of engine.
	const std::size_t numOfInputs = engine.getInputIndexes().size();
	const std::size_t numOfOutputs = engine.getOutputIndexes().size();
	const std::size_t numOfRoots = engine.getRootIndexes().size();
	assertEquals(numOfInputs, 0);
	assertEquals(numOfOutputs, 0);
	assertEquals(numOfRoots, 0);
}

void test3_removeWire(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *wire = circuit.find("wire");

	// Remove wire.
	circuit.remove(wire);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Sizes of engine.
	const std::size_t numOfInputs = engine.getInputIndexes().size();
	const std::size_t numOfOutputs = engine.getOutputIndexes().size();
	const std::size_t numOfRoots = engine.getRootIndexes().size();
	assertEquals(numOfInputs, 1);
	assertEquals(numOfOutputs, 0);
	assertEquals(numOfRoots, 0);
}

int main()
{
	// Run all tests.
	PerformTest(generator, checker, test1_removeOutput);
	PerformTest(generator, checker, test2_removeInput);
	PerformTest(generator, checker, test3_removeWire);
}
