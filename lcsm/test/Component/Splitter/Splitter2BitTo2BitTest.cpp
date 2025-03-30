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

int main()
{
	PerformTest(generator, checker, test);
}
