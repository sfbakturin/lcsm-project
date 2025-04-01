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

using TestDataWithDecimalPoint = std::pair< DataBits, DataBits >;

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

	// Change circuit's elements.
	const Width width = Width::Bit4;
	inputModel->setWidth(width);
	outputModel->setWidth(width);

	// Create and connect Pin for decimal point.
	model::Pin *decimalPoint = circuit.createPin(false, "decimal point");
	model::Wire *wire3 = circuit.connect(decimalPoint, digit, model::Digit::Port::WiringDecimalPoint);
	wire3->setName("wire3");

	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	const Identifier digitId = digit->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();
	const Identifier wire3Id = wire3->id();
	const Identifier decimalPointId = decimalPoint->id();

	// Testing data: expected==actual value with decimal point.
	// clang-format off
	const std::vector< TestDataWithDecimalPoint > testdatas = {
		{ { T, F, F, F }, { T } },
		{ { T, F, T, F }, { F } },
		{ { T, F, T, F }, { T } },
		{ { T, T, F, T }, { F } },
		{ { T, T, T, F }, { F } },
		{ { F, T, T, F }, { T } },
	};
	// clang-format on

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (const TestDataWithDecimalPoint &testdata : testdatas)
	{
		// Test's data.
		const DataBits &expectedData = testdata.first;
		const DataBits &expectedDecimalPoint = testdata.second;

		// Put value.
		state.putValue(inputId, expectedData);
		state.putValue(decimalPointId, expectedDecimalPoint);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &actualInput = state.valueOf(inputId);
		const DataBits &actualOutput = state.valueOf(outputId);
		const DataBits &actualDigitData = state.valueOf(digitId, 0);
		const DataBits &actualDigitDecimalPoint = state.valueOf(digitId, 1);
		const DataBits &actualWire1 = state.valueOf(wire1Id);
		const DataBits &actualWire2 = state.valueOf(wire2Id);
		const DataBits &actualWire3 = state.valueOf(wire3Id);
		assertEquals(actualInput, expectedData);
		assertEquals(actualOutput, expectedData);
		assertEquals(actualDigitData, expectedData);
		assertEquals(actualDigitDecimalPoint, expectedDecimalPoint);
		assertEquals(actualWire1, expectedData);
		assertEquals(actualWire2, expectedData);
		assertEquals(actualWire3, expectedDecimalPoint);
		std::cout << "<input = " << actualInput << "> --> <wire1 = " << actualWire1 << "> --> <output = " << actualOutput << ">\n";
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
