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

#include <vector>

using namespace lcsm::testing;
using namespace lcsm_tests;

static void test(LCSMCircuit &circuit)
{
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
	PerformTest(generator, checker, test);
}
