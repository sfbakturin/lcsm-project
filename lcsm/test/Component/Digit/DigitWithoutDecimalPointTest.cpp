#include "DigitTesting.h"
#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm_tests;

using TestData = DataBits;

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *input = circuit.find("input");
	Component *output = circuit.find("output");
	Component *digit = circuit.find("digit");
	Component *wire1 = circuit.find("wire1");
	Component *wire2 = circuit.find("wire2");

	// Extract models.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Pin *outputModel = static_cast< model::Pin * >(output);
	model::Digit *digitModel = static_cast< model::Digit * >(digit);

	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	const Identifier digitId = digit->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();

	// Change circuit's elements.
	const Width width = Width::Bit4;
	inputModel->setWidth(width);
	outputModel->setWidth(width);
	digitModel->setHasDecimalPoint(false);

	// Testing data: expected==actual value.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ T, F, F, F },
		{ T, F, T, F },
		{ T, F, T, F },
		{ T, T, F, T },
		{ T, T, T, F },
		{ F, T, T, F },
	};
	// clang-format on

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (const TestData &testdata : testdatas)
	{
		// Test's data.
		const DataBits &expected = testdata;

		// Put value.
		state.putValue(inputId, expected);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &actualInput = state.valueOf(inputId);
		const DataBits &actualOutput = state.valueOf(outputId);
		const DataBits &actualDigit = state.valueOf(digitId);
		const DataBits &actualWire1 = state.valueOf(wire1Id);
		const DataBits &actualWire2 = state.valueOf(wire2Id);
		assertEquals(actualInput, expected);
		assertEquals(actualOutput, expected);
		assertEquals(actualDigit, expected);
		assertEquals(actualWire1, expected);
		assertEquals(actualWire2, expected);
		std::cout << "<input = " << actualInput << "> --> <wire1 = " << actualWire1 << "> --> <output = " << actualOutput << ">\n";
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
