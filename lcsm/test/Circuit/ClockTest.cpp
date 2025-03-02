#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>

#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace lcsm;

static LCSMCircuit getCircuit()
{
	LCSMCircuit circuit("ClockTest");

	// Create Clock and output Pin model objects.
	model::Clock *clock = circuit.createClock("clock");
	model::Pin *pin = circuit.createPin(true, "pin");

	// Connect Clock to output Pin.
	circuit.connect(clock, pin, "wire");

	return circuit;
}

static void assertNotNull(void *ptr, const char *s)
{
	if (ptr == NULL)
	{
		throw std::runtime_error(s);
	}
}

static void assertEquals(const DataBits &actual, DataBits &&expected)
{
	if (actual != expected)
	{
		throw std::logic_error("Actual != expected");
	}
}

int main()
{
	LCSMCircuit circuit = getCircuit();

	// Get components.
	Circuit *wire = circuit.find("wire");
	Circuit *pin = circuit.find("pin");

	// Check if they are not nullptrs.
	// assertNotNull(wire, "Wire element is not found");
	assertNotNull(pin, "Output Pin element is not found");

	// Get ids.
	Identifier wireId = wire->id();
	Identifier pinId = pin->id();

	// Construct engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Tick once.
	state.tick();

	// Check.
	std::cout << state.valueOf(pinId) << '\n';

	// Tick once.
	state.tick();

	// Check.
	std::cout << state.valueOf(pinId) << '\n';
}
