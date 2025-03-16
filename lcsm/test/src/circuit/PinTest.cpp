#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
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

#include <array>
#include <iostream>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;
static constexpr Bit U = Bit::Undefined;

const std::array< Bit, 2 > bits1 = { T, F };
const std::array< Bit, 2 > bits2 = { F, T };

static LCSMCircuit generator1()
{
	LCSMCircuit circuit("PinCircuitTest1");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");

	// Make connections.
	model::Wire *wire = circuit.connect(input, output);
	wire->setName("wire");

	return circuit;
}

static void checker1(LCSMCircuit &circuit)
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

static LCSMCircuit generator2()
{
	LCSMCircuit circuit("PinCircuitTest2");

	// Create all needed circuit elements.
	model::Pin *input1 = circuit.createPin(false, "input1");
	model::Pin *input2 = circuit.createPin(false, "input2");

	// Make connections.
	model::Wire *wire = circuit.connect(input1, input2);
	wire->setName("wire");

	return circuit;
}

static void checker2(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input1 = circuit.find("input1");
	Circuit *input2 = circuit.find("input2");
	Circuit *wire = circuit.find("wire");

	// Check element's types.
	assertType(input1, CircuitType::Pin);
	assertType(input2, CircuitType::Pin);
	assertType(wire, CircuitType::Wire);
}

static LCSMCircuit generator3()
{
	LCSMCircuit circuit("PinCircuitTest3");

	// Create all needed circuit elements.
	model::Pin *input1 = circuit.createPin(false, "input1");
	model::Pin *input2 = circuit.createPin(false, "input2");
	model::Pin *output1 = circuit.createPin(true, "output1");
	model::Pin *output2 = circuit.createPin(true, "output2");

	// Make connections.
	model::Wire *wire1 = circuit.connect(input1, input2);
	model::Wire *wire2 = circuit.connect(wire1, output1);
	model::Wire *wire3 = circuit.connect(wire1, output2);
	wire1->setName("wire1");
	wire2->setName("wire2");
	wire3->setName("wire3");

	return circuit;
}

static void checker3(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input1 = circuit.find("input1");
	Circuit *input2 = circuit.find("input2");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");
	Circuit *wire3 = circuit.find("wire3");

	// Check element's types.
	assertType(input1, CircuitType::Pin);
	assertType(input2, CircuitType::Pin);
	assertType(output1, CircuitType::Pin);
	assertType(output2, CircuitType::Pin);
	assertType(wire1, CircuitType::Wire);
	assertType(wire2, CircuitType::Wire);
	assertType(wire3, CircuitType::Wire);
}

static void test0_pretest()
{
	// Generators
	const GeneratorTy g1 = generator1;
	const GeneratorTy g2 = generator2;
	const GeneratorTy g3 = generator3;

	// Checkers.
	const CheckerTy c1 = checker1;
	const CheckerTy c2 = checker2;
	const CheckerTy c3 = checker3;

	// Pre test.
	preTest(g1, c1);
	preTest(g2, c2);
	preTest(g3, c3);
}

static void test1()
{
	LCSMCircuit circuit = generator1();

	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *wire = circuit.find("wire");

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

static void test2()
{
	LCSMCircuit circuit = generator2();

	// Find circuits.
	Circuit *input1 = circuit.find("input1");
	Circuit *input2 = circuit.find("input2");
	Circuit *wire = circuit.find("wire");

	// Extract models.
	model::Pin *input1Model = static_cast< model::Pin * >(input1);
	model::Pin *input2Model = static_cast< model::Pin * >(input2);

	// Indexes.
	const Identifier input1Id = input1->id();
	const Identifier input2Id = input2->id();
	const Identifier wireId = wire->id();

	// Test!
	for (Width width : Widths)
	{
		// Change model's settings.
		input1Model->setWidth(width);
		input2Model->setWidth(width);

		// Generate physical engine.
		LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

		// Fork new state.
		LCSMState state = engine.fork();

		for (Bit bit : bits1)
		{
			// Testing data.
			const DataBits testdata{ width, bit };

			// Put values.
			state.putValue(input1Id, testdata);
			state.putValue(input2Id, testdata);

			// Step once.
			state.tick();

			// Assert.
			const DataBits &input1Actual = state.valueOf(input1Id);
			const DataBits &input2Actual = state.valueOf(input2Id);
			const DataBits &wireActual = state.valueOf(wireId);
			assertEquals(input1Actual, testdata);
			assertEquals(input2Actual, testdata);
			assertEquals(wireActual, testdata);

			// Printout.
			std::cout << "<pin = " << input1Actual << "> --> <wire = " << wireActual << "> --> <pin = " << input2Actual << ">\n";
		}

		for (std::size_t i = 0; i < 2; i++)
		{
			// Testing data.
			const DataBits input1Testdata{ width, bits1[i] };
			const DataBits input2Testdata{ width, bits2[i] };
			const DataBits wireTestData{ width, Strength::StrongDrive, U };

			// Put values.
			state.putValue(input1Id, input1Testdata);
			state.putValue(input2Id, input2Testdata);

			// Step once.
			state.tick();

			// Assert.
			const DataBits &input1Actual = state.valueOf(input1Id);
			const DataBits &input2Actual = state.valueOf(input2Id);
			const DataBits &wireActual = state.valueOf(wireId);
			assertEquals(input1Actual, input1Testdata);
			assertEquals(input2Actual, input2Testdata);
			assertEquals(wireActual, wireTestData);

			// Printout.
			std::cout << "<pin = " << input1Actual << "> --> <wire = " << wireActual << "> --> <pin = " << input2Actual << ">\n";
		}
	}
}

static void test3()
{
	LCSMCircuit circuit = generator3();

	// Find circuits.
	Circuit *input1 = circuit.find("input1");
	Circuit *input2 = circuit.find("input2");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");
	Circuit *wire3 = circuit.find("wire3");

	// Extract models.
	model::Pin *input1Model = static_cast< model::Pin * >(input1);
	model::Pin *input2Model = static_cast< model::Pin * >(input2);
	model::Pin *output1Model = static_cast< model::Pin * >(output1);
	model::Pin *output2Model = static_cast< model::Pin * >(output2);

	// Indexes.
	const Identifier input1Id = input1->id();
	const Identifier input2Id = input2->id();
	const Identifier output1Id = output1->id();
	const Identifier output2Id = output2->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();
	const Identifier wire3Id = wire3->id();

	// Test!
	for (Width width : Widths)
	{
		// Change model's settings.
		input1Model->setWidth(width);
		input2Model->setWidth(width);
		output1Model->setWidth(width);
		output2Model->setWidth(width);

		// Generate physical engine.
		LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

		// Fork new state.
		LCSMState state = engine.fork();

		// Test 1. When there is all values normal.
		for (Bit bit : bits1)
		{
			// Testing data.
			const DataBits testdata{ width, bit };

			// Put values.
			state.putValue(input1Id, testdata);
			state.putValue(input2Id, testdata);

			// Step once.
			state.tick();

			// Assert.
			const DataBits &input1Actual = state.valueOf(input1Id);
			const DataBits &input2Actual = state.valueOf(input2Id);
			const DataBits &output1Actual = state.valueOf(output1Id);
			const DataBits &output2Actual = state.valueOf(output2Id);
			const DataBits &wire1Actual = state.valueOf(wire1Id);
			const DataBits &wire2Actual = state.valueOf(wire2Id);
			const DataBits &wire3Actual = state.valueOf(wire3Id);
			assertEquals(input1Actual, testdata);
			assertEquals(input2Actual, testdata);
			assertEquals(output1Actual, testdata);
			assertEquals(output2Actual, testdata);
			assertEquals(wire1Actual, testdata);
			assertEquals(wire2Actual, testdata);
			assertEquals(wire3Actual, testdata);
		}

		// Test 2. When there is U.
		for (std::size_t i = 0; i < 2; i++)
		{
			// Testing data.
			const DataBits input1Testdata{ width, bits1[i] };
			const DataBits input2Testdata{ width, bits2[i] };
			const DataBits outputTestdata{ width, Strength::StrongDrive, U };

			// Put values.
			state.putValue(input1Id, input1Testdata);
			state.putValue(input2Id, input2Testdata);

			// Step once.
			state.tick();

			// Assert.
			const DataBits &input1Actual = state.valueOf(input1Id);
			const DataBits &input2Actual = state.valueOf(input2Id);
			const DataBits &output1Actual = state.valueOf(output1Id);
			const DataBits &output2Actual = state.valueOf(output2Id);
			const DataBits &wire1Actual = state.valueOf(wire1Id);
			const DataBits &wire2Actual = state.valueOf(wire2Id);
			const DataBits &wire3Actual = state.valueOf(wire3Id);
			assertEquals(input1Actual, input1Testdata);
			assertEquals(input2Actual, input2Testdata);
			assertEquals(output1Actual, outputTestdata);
			assertEquals(output2Actual, outputTestdata);
			assertEquals(wire1Actual, outputTestdata);
			assertEquals(wire2Actual, outputTestdata);
			assertEquals(wire3Actual, outputTestdata);
		}
	}
}

int main()
{
	test0_pretest();
	test1();
	test2();
	test3();
}
