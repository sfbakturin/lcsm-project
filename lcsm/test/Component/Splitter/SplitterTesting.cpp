#include "SplitterTesting.h"

#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Splitter.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Value.h>

#include <iostream>
#include <vector>

using namespace lcsm::testing;

LCSMCircuit lcsm_tests::generator()
{
	LCSMCircuit circuit("SplitterTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Splitter *splitter = circuit.createSplitter("splitter");
	model::Pin *output1 = circuit.createPin(true, "output1");
	model::Pin *output2 = circuit.createPin(true, "output2");

	// Make connections.
	circuit.connect(input, splitter);
	circuit.connect(splitter, model::Splitter::Port::Out0, output1);
	circuit.connect(splitter, model::Splitter::Port::Out1, output2);

	return circuit;
}

void lcsm_tests::checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *splitter = circuit.find("splitter");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");

	// Check element's types.
	assertType(input, CircuitType::Pin);
	assertType(splitter, CircuitType::Splitter);
	assertType(output1, CircuitType::Pin);
	assertType(output2, CircuitType::Pin);
}

void lcsm_tests::test2bit(LCSMState &state, const Circuit *input, const Circuit *output1, const Circuit *output2, const std::vector< TestData2bit > &testdatas)
{
	const Identifier inputId = input->id();
	const Identifier output1Id = output1->id();
	const Identifier output2Id = output2->id();
	for (const TestData2bit &testdata : testdatas)
	{
		// Test's data.
		const DataBits &expectedInput = std::get< 0 >(testdata);
		const DataBits &expectedOutput1 = std::get< 1 >(testdata);
		const DataBits &expectedOutput2 = std::get< 2 >(testdata);

		// Put value to input.
		state.putValue(inputId, expectedInput);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &actualInput = state.valueOf(inputId);
		const DataBits &actualOutput1 = state.valueOf(output1Id);
		const DataBits &actualOutput2 = state.valueOf(output2Id);
		assertEquals(actualInput, expectedInput);
		assertEquals(actualOutput1, expectedOutput1);
		assertEquals(actualOutput2, expectedOutput2);

		// Printout values on outputs.
		std::cout << "input = " << actualInput << " => output1 = " << expectedOutput1 << ", output2 = " << expectedOutput2 << '\n';
	}
}

void lcsm_tests::test3bit(
	LCSMState &state,
	const Circuit *input,
	const Circuit *output1,
	const Circuit *output2,
	const Circuit *output3,
	const std::vector< TestData3bit > &testdatas)
{
	const Identifier inputId = input->id();
	const Identifier output1Id = output1->id();
	const Identifier output2Id = output2->id();
	const Identifier output3Id = output3->id();
	for (const TestData3bit &testdata : testdatas)
	{
		// Test's data.
		const DataBits &expectedInput = std::get< 0 >(testdata);
		const DataBits &expectedOutput1 = std::get< 1 >(testdata);
		const DataBits &expectedOutput2 = std::get< 2 >(testdata);
		const DataBits &expectedOutput3 = std::get< 3 >(testdata);

		// Put value to input.
		state.putValue(inputId, expectedInput);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &actualInput = state.valueOf(inputId);
		const DataBits &actualOutput1 = state.valueOf(output1Id);
		const DataBits &actualOutput2 = state.valueOf(output2Id);
		const DataBits &actualOutput3 = state.valueOf(output3Id);
		assertEquals(actualInput, expectedInput);
		assertEquals(actualOutput1, expectedOutput1);
		assertEquals(actualOutput2, expectedOutput2);
		assertEquals(actualOutput3, expectedOutput3);

		// Printout values on outputs.
		std::cout << "input = " << actualInput << " => output1 = " << expectedOutput1
				  << ", output2 = " << expectedOutput2 << ", output3 = " << expectedOutput3 << '\n';
	}
}
