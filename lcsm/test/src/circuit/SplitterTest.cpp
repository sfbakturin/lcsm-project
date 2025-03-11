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
#include <tuple>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

using TestData2bit = std::tuple< DataBits, DataBits, DataBits >;
using TestData3bit = std::tuple< DataBits, DataBits, DataBits, DataBits >;

static LCSMCircuit generator()
{
	LCSMCircuit circuit("SplitterCircuitTest");

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

static void checker(LCSMCircuit &circuit)
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

static void test0_pretest()
{
	const GeneratorTy g = generator;
	const CheckerTy c = checker;
	preTest(g, c);
}

static void
	test2bit(LCSMState &state, const Circuit *input, const Circuit *output1, const Circuit *output2, const std::vector< TestData2bit > &testdatas)
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

static void test1_2bitTo2bit()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");

	// Change model's settings.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	inputModel->setWidth(Width::Bit2);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Inputs and expected: at 0 - input, at 1 - expected output1, at 2 - expected output2.
	// clang-format off
	const std::vector< TestData2bit > testdatas = {
		{ { F, F }, { F }, { F } },
		{ { F, T }, { F }, { T } },
		{ { T, F }, { T }, { F } },
		{ { T, T }, { T }, { T } },
	};
	// clang-format on

	// Test!
	test2bit(state, input, output1, output2, testdatas);
}

static void test2_3bitTo2bit()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *splitter = circuit.find("splitter");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");

	// Change Splitter: widthIn = 3bit.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Splitter *splitterModel = static_cast< model::Splitter * >(splitter);
	model::Pin *output1Model = static_cast< model::Pin * >(output1);
	splitterModel->setWidthIn(Width::Bit3);
	inputModel->setWidth(Width::Bit3);
	output1Model->setWidth(Width::Bit2);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Inputs and expected: at 0 - input, at 1 - expected output1, at 2 - expected output2.
	// clang-format off
	const std::vector< TestData2bit > testdatas = {
		{ { F, F, F }, { F, F }, { F } },
		{ { F, F, T }, { F, F }, { T } },
		{ { F, T, F }, { F, T }, { F } },
		{ { F, T, T }, { F, T }, { T } },
		{ { T, F, F }, { T, F }, { F } },
		{ { T, F, T }, { T, F }, { T } },
		{ { T, T, F }, { T, T }, { F } },
		{ { T, T, T }, { T, T }, { T } }
	};
	// clang-format on

	// Test!
	test2bit(state, input, output1, output2, testdatas);
}

static void test3_4bitTo2bit()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *splitter = circuit.find("splitter");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");

	// Change Splitter: widthIn = 4bit.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Splitter *splitterModel = static_cast< model::Splitter * >(splitter);
	model::Pin *output1Model = static_cast< model::Pin * >(output1);
	model::Pin *output2Model = static_cast< model::Pin * >(output2);
	splitterModel->setWidthIn(Width::Bit4);
	inputModel->setWidth(Width::Bit4);
	output1Model->setWidth(Width::Bit2);
	output2Model->setWidth(Width::Bit2);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Inputs and expected: at 0 - input, at 1 - expected output1, at 2 - expected output2.
	// clang-format off
	const std::vector< TestData2bit > testdatas = {
		{ { F, F, F, F }, { F, F }, { F, F } },
		{ { F, F, F, T }, { F, F }, { F, T } },
		{ { F, F, T, F }, { F, F }, { T, F } },
		{ { F, F, T, T }, { F, F }, { T, T } },
		{ { F, T, F, F }, { F, T }, { F, F } },
		{ { F, T, F, T }, { F, T }, { F, T } },
		{ { F, T, T, F }, { F, T }, { T, F } },
		{ { F, T, T, T }, { F, T }, { T, T } },
		{ { T, F, F, F }, { T, F }, { F, F } },
		{ { T, F, F, T }, { T, F }, { F, T } },
		{ { T, F, T, F }, { T, F }, { T, F } },
		{ { T, F, T, T }, { T, F }, { T, T } },
		{ { T, T, F, F }, { T, T }, { F, F } },
		{ { T, T, F, T }, { T, T }, { F, T } },
		{ { T, T, T, F }, { T, T }, { T, F } },
		{ { T, T, T, T }, { T, T }, { T, T } },
	};
	// clang-format on

	// Test!
	test2bit(state, input, output1, output2, testdatas);
}

static void
	test3bit(LCSMState &state, const Circuit *input, const Circuit *output1, const Circuit *output2, const Circuit *output3, const std::vector< TestData3bit > &testdatas)
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

static void test4_4bitTo3bit()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *splitter = circuit.find("splitter");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");

	// Change Splitter: widthIn = 4bit, widthOut = 3bit.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Splitter *splitterModel = static_cast< model::Splitter * >(splitter);
	model::Pin *output1Model = static_cast< model::Pin * >(output1);
	splitterModel->setWidthIn(Width::Bit4);
	inputModel->setWidth(Width::Bit4);
	splitterModel->setWidthOut(Width::Bit3);
	output1Model->setWidth(Width::Bit2);

	// Create additional output and connect to this output.
	model::Pin *output3 = circuit.createPin(true, "output3");
	circuit.connect(splitter, model::Splitter::Port::Out2, output3);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Inputs and expected: at 0 - input, at 1 - expected output1, at 2 - expected output2.
	// clang-format off
	const std::vector< TestData3bit > testdatas = {
		{ { F, F, F, F }, { F, F }, { F }, { F } },
		{ { F, F, F, T }, { F, F }, { F }, { T } },
		{ { F, F, T, F }, { F, F }, { T }, { F } },
		{ { F, F, T, T }, { F, F }, { T }, { T } },
		{ { F, T, F, F }, { F, T }, { F }, { F } },
		{ { F, T, F, T }, { F, T }, { F }, { T } },
		{ { F, T, T, F }, { F, T }, { T }, { F } },
		{ { F, T, T, T }, { F, T }, { T }, { T } },
		{ { T, F, F, F }, { T, F }, { F }, { F } },
		{ { T, F, F, T }, { T, F }, { F }, { T } },
		{ { T, F, T, F }, { T, F }, { T }, { F } },
		{ { T, F, T, T }, { T, F }, { T }, { T } },
		{ { T, T, F, F }, { T, T }, { F }, { F } },
		{ { T, T, F, T }, { T, T }, { F }, { T } },
		{ { T, T, T, F }, { T, T }, { T }, { F } },
		{ { T, T, T, T }, { T, T }, { T }, { T } },
	};
	// clang-format on

	// Test!
	test3bit(state, input, output1, output2, output3, testdatas);
}

int main()
{
	// Run all tests.
	test0_pretest();
	test1_2bitTo2bit();
	test2_3bitTo2bit();
	test3_4bitTo2bit();
	test4_4bitTo3bit();
}
