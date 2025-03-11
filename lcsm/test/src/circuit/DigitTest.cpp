#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Digit.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>
#include <utility>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

using TestData = DataBits;
using TestDataWithDecimalPoint = std::pair< DataBits, DataBits >;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

static LCSMCircuit generator()
{
	LCSMCircuit circuit("DigitCircuitTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");
	model::Digit *digit = circuit.createDigit("digit");

	// Make connections.
	model::Wire *wire1 = circuit.connect(input, output);
	model::Wire *wire2 = circuit.connect(wire1, digit);
	wire1->setName("wire1");
	wire2->setName("wire2");

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *digit = circuit.find("digit");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");

	// Check element's types.
	assertType(input, CircuitType::Pin);
	assertType(output, CircuitType::Pin);
	assertType(digit, CircuitType::Digit);
	assertType(wire1, CircuitType::Wire);
	assertType(wire2, CircuitType::Wire);
}

static void test0_pretest()
{
	const GeneratorTy g = generator;
	const CheckerTy c = checker;
	preTest(g, c);
}

static void
	test1(LCSMState &state, const Circuit *input, const Circuit *output, const Circuit *digit, const Circuit *wire1, const Circuit *wire2, const DataBits &expected)
{
	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	const Identifier digitId = digit->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();

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

static void test1_withoutDecimalPoint()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *digit = circuit.find("digit");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");

	// Extract models.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Pin *outputModel = static_cast< model::Pin * >(output);
	model::Digit *digitModel = static_cast< model::Digit * >(digit);

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

		// Run single test.
		test1(state, input, output, digit, wire1, wire2, expected);
	}
}

static void test2(
	LCSMState &state,
	const Circuit *input,
	const Circuit *output,
	const Circuit *digit,
	const Circuit *wire1,
	const Circuit *wire2,
	const Circuit *wire3,
	const Circuit *decimalPoint,
	const DataBits &expectedData,
	const DataBits &expectedDecimalPoint)
{
	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	const Identifier digitId = digit->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();
	const Identifier wire3Id = wire3->id();
	const Identifier decimalPointId = decimalPoint->id();

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

static void test2_withDecimalPoint()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *digit = circuit.find("digit");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");

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

		// Run single test.
		test2(state, input, output, digit, wire1, wire2, wire3, decimalPoint, expectedData, expectedDecimalPoint);
	}
}

int main()
{
	// Run all tests.
	test0_pretest();
	test1_withoutDecimalPoint();
	test2_withDecimalPoint();
}
