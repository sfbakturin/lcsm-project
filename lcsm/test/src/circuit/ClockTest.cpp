#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>
#include <utility>
#include <vector>

using namespace lcsm;
using namespace lcsm::verilog;
using namespace lcsm::testing;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

using TestData = std::pair< DataBits, DataBits >;

static LCSMCircuit generator()
{
	LCSMCircuit circuit;
	model::Clock *clock = circuit.createClock("clock");
	model::Pin *pin = circuit.createPin(true, "pin");
	circuit.connect(clock, pin);
	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *clock = circuit.find("clock");
	Circuit *pin = circuit.find("pin");

	// Check element's types.
	assertType(clock, CircuitType::Clock);
	assertType(pin, CircuitType::Pin);
}

static void test(LCSMState &state, const Circuit *clock, const Circuit *pin, const std::vector< TestData > &testdatas)
{
	for (const TestData &testdata : testdatas)
	{
		// Step once.
		state.tick();

		// Assert.
		const DataBits &actualClock = state.valueOf(clock->id());
		const DataBits &actualPin = state.valueOf(pin->id());
		const DataBits &expectedClock = testdata.first;
		const DataBits &expectedPin = testdata.second;
		assertEquals(actualClock, expectedClock);
		assertEquals(actualPin, expectedPin);

		// Printout values on outputs.
		std::cout << "clock = " << actualClock << " => pin = " << actualPin << '\n';
	}
}

static void test1_low1high1(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *clock = circuit.find("clock");
	Circuit *pin = circuit.find("pin");

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Expecteds: at 0 - Clock's value, at 1 - Pin's.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { T }, { T } },
		{ { F }, { F } },
		{ { T }, { T } },
		{ { F }, { F } },
	};
	// clang-format on

	// Test!
	test(state, clock, pin, testdatas);
}

static void test2_low2high1(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *clock = circuit.find("clock");
	Circuit *pin = circuit.find("pin");

	// Change Clock.
	model::Clock *clockModel = static_cast< model::Clock * >(clock);
	clockModel->setLowDuration(2);
	clockModel->setHighDuration(1);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Expecteds: at 0 - Clock's value, at 1 - Pin's.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { T }, { T } },
		{ { F }, { F } },
		{ { F }, { F } },
		{ { T }, { T } },
		{ { F }, { F } },
		{ { F }, { F } },
		{ { T }, { T } },
	};
	// clang-format on

	// Test!
	test(state, clock, pin, testdatas);
}

static void test3_low2high2(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *clock = circuit.find("clock");
	Circuit *pin = circuit.find("pin");

	// Change Clock.
	model::Clock *clockModel = static_cast< model::Clock * >(clock);
	clockModel->setLowDuration(2);
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
		{ { F }, { F } },
		{ { F }, { F } },
		{ { T }, { T } },
		{ { T }, { T } },
		{ { F }, { F } },
		{ { F }, { F } },
	};
	// clang-format on

	// Test!
	test(state, clock, pin, testdatas);
}

static void test4_low1high0(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *clock = circuit.find("clock");
	Circuit *pin = circuit.find("pin");

	// Change Clock.
	model::Clock *clockModel = static_cast< model::Clock * >(clock);
	clockModel->setLowDuration(1);
	clockModel->setHighDuration(0);

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Expecteds: at 0 - Clock's value, at 1 - Pin's.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { F }, { F } },
		{ { F }, { F } },
		{ { F }, { F } },
		{ { F }, { F } },
	};
	// clang-format on

	// Test!
	test(state, clock, pin, testdatas);
}

static void test5_low0high2(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *clock = circuit.find("clock");
	Circuit *pin = circuit.find("pin");

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
	test(state, clock, pin, testdatas);
}

int main()
{
	// Run all tests.
	PerformTest(generator, checker, test1_low1high1);
	PerformTest(generator, checker, test2_low2high1);
	PerformTest(generator, checker, test3_low2high2);
	PerformTest(generator, checker, test4_low1high0);
	PerformTest(generator, checker, test5_low0high2);
}
