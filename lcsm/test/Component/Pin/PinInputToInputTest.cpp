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

#include <iostream>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;
static constexpr Bit U = Bit::Undefined;

const std::array< Bit, 2 > bits1 = { T, F };
const std::array< Bit, 2 > bits2 = { F, T };

static LCSMCircuit generator()
{
	LCSMCircuit circuit("PinTest");

	// Create all needed circuit elements.
	model::Pin *input1 = circuit.createPin(false, "input1");
	model::Pin *input2 = circuit.createPin(false, "input2");

	// Make connections.
	model::Wire *wire = circuit.connect(input1, input2);
	wire->setName("wire");

	return circuit;
}

static void checker(LCSMCircuit &circuit)
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

static void test(LCSMCircuit &circuit)
{
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

int main()
{
	PerformTest(generator, checker, test);
}
