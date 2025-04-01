#include "ClockTesting.h"
#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <vector>

using namespace lcsm;
using namespace lcsm::verilog;
using namespace lcsm::testing;
using namespace lcsm_tests;

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *clock = circuit.find("clock");
	Component *pin = circuit.find("pin");

	// Change Clock.
	model::Clock *clockModel = static_cast< model::Clock * >(clock);
	clockModel->setLowDuration(0);
	clockModel->setHighDuration(2);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Expecteds: at 0 - Clock's value, at 1 - Pin's.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { T }, { T } },
		{ { T }, { T } },
		{ { T }, { T } },
		{ { T }, { T } },
	};
	// clang-format on

	// Test!
	singleTest(state, clock, pin, testdatas);
}

int main()
{
	PerformTest(generator, checker, test);
}
