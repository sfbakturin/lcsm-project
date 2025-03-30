#include "ClockTesting.h"

#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>

#include <iostream>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;

LCSMCircuit lcsm_tests::generator()
{
	LCSMCircuit circuit("ClockTest");

	// Create all needed components.
	model::Clock *clock = circuit.createClock("clock");
	model::Pin *pin = circuit.createPin(true, "pin");

	// Make connections.
	circuit.connect(clock, pin);

	return circuit;
}

void lcsm_tests::checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *clock = circuit.find("clock");
	Circuit *pin = circuit.find("pin");

	// Check element's types.
	assertType(clock, CircuitType::Clock);
	assertType(pin, CircuitType::Pin);
}

void lcsm_tests::singleTest(LCSMState &state, const Circuit *clock, const Circuit *pin, const std::vector< TestData > &testdatas)
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
