#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Probe.h>
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

using TestData = std::pair< Width, DataBits >;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

static LCSMCircuit generator()
{
	LCSMCircuit circuit("ProbeCircuitTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");
	model::Probe *probe = circuit.createProbe("probe");

	// Make connections.
	model::Wire *wire1 = circuit.connect(input, output);
	model::Wire *wire2 = circuit.connect(wire1, probe);
	wire1->setName("wire1");
	wire2->setName("wire2");

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *probe = circuit.find("probe");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");

	// Check element's types.
	assertType(input, CircuitType::Pin);
	assertType(output, CircuitType::Pin);
	assertType(probe, CircuitType::Probe);
	assertType(wire1, CircuitType::Wire);
	assertType(wire2, CircuitType::Wire);
}

static void
	testImpl(LCSMState &state, const Circuit *input, const Circuit *output, const Circuit *probe, const Circuit *wire1, const Circuit *wire2, const DataBits &expected)
{
	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	const Identifier probeId = probe->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();

	// Put value.
	state.putValue(inputId, expected);

	// Step once.
	state.tick();

	// Assert.
	const DataBits &actualInput = state.valueOf(inputId);
	const DataBits &actualOutput = state.valueOf(outputId);
	const DataBits &actualProbe = state.valueOf(probeId);
	const DataBits &actualWire1 = state.valueOf(wire1Id);
	const DataBits &actualWire2 = state.valueOf(wire2Id);
	assertEquals(actualInput, expected);
	assertEquals(actualOutput, expected);
	assertEquals(actualProbe, expected);
	assertEquals(actualWire1, expected);
	assertEquals(actualWire2, expected);
	std::cout << "<input = " << actualInput << "> --> <wire1 = " << actualWire1 << "> --> <output = " << actualOutput << ">\n";
}

static void test1(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *probe = circuit.find("probe");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");

	// Extract models.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Pin *outputModel = static_cast< model::Pin * >(output);

	// Testing data: expected==actual value.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ Width::Bit1, { T } },
		{ Width::Bit2, { T, F } },
		{ Width::Bit3, { T, F, T } },
		{ Width::Bit3, { T, T, F } },
		{ Width::Bit4, { T, T, T, F } },
		{ Width::Bit4, { F, T, T, F } },
		{ Width::Byte, { T, T, F, F, T, T, F, F } }
	};
	// clang-format on

	// Test!
	for (const TestData &testdata : testdatas)
	{
		// Test's data.
		const Width width = testdata.first;
		const DataBits &expected = testdata.second;

		// Change model's settings.
		inputModel->setWidth(width);
		outputModel->setWidth(width);

		// Generate physical engine.
		LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

		// Fork new state.
		LCSMState state = engine.fork();

		// Run single test.
		testImpl(state, input, output, probe, wire1, wire2, expected);
	}
}

int main()
{
	// Run all tests.
	PerformTest(generator, checker, test1);
}
