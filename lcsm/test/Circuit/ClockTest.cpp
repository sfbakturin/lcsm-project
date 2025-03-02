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
#include <lcsm/Verilog/Bit.h>

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

static void assertType(const Circuit *circuit, CircuitType type)
{
	if (circuit->circuitType() != type)
	{
		throw std::logic_error("Circuit is not desired type");
	}
}

static void test1()
{
	LCSMCircuit circuit = getCircuit();

	// Get components.
	Circuit *wire = circuit.find("wire");
	Circuit *pin = circuit.find("pin");

	// Check if they are not nullptrs.
	assertNotNull(wire, "Wire element is not found");
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
	std::cout << "[1] pin = " << state.valueOf(pinId) << " wire = " << state.valueOf(wireId) << '\n';
	assertEquals(state.valueOf(pinId), DataBits(Width::Bit1, verilog::Bit::True));
	assertEquals(state.valueOf(wireId), DataBits(Width::Bit1, verilog::Bit::True));

	// Tick once.
	state.tick();

	// Check.
	std::cout << "[2] pin = " << state.valueOf(pinId) << " wire = " << state.valueOf(wireId) << '\n';
	assertEquals(state.valueOf(pinId), DataBits(Width::Bit1, verilog::Bit::False));
	assertEquals(state.valueOf(wireId), DataBits(Width::Bit1, verilog::Bit::False));
}

static void test2()
{
	LCSMCircuit circuit = getCircuit();

	// Get components.
	Circuit *clock = circuit.find("clock");
	Circuit *wire = circuit.find("wire");
	Circuit *pin = circuit.find("pin");

	// Check if they are not nullptrs.
	assertNotNull(clock, "Clock element is not found");
	assertNotNull(wire, "Wire element is not found");
	assertNotNull(pin, "Output Pin element is not found");

	// Check types.
	assertType(clock, CircuitType::Clock);
	assertType(wire, CircuitType::Wire);
	assertType(pin, CircuitType::Pin);

	// Set new properties for Clock.
	model::Clock *clockElement = static_cast< model::Clock * >(clock);
	clockElement->setHighDuration(2);
	clockElement->setLowDuration(2);

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
	std::cout << "[3] pin = " << state.valueOf(pinId) << " wire = " << state.valueOf(wireId) << '\n';
	assertEquals(state.valueOf(pinId), DataBits(Width::Bit1, verilog::Bit::True));
	assertEquals(state.valueOf(wireId), DataBits(Width::Bit1, verilog::Bit::True));

	// Tick once.
	state.tick();

	// Check.
	std::cout << "[4] pin = " << state.valueOf(pinId) << " wire = " << state.valueOf(wireId) << '\n';
	assertEquals(state.valueOf(pinId), DataBits(Width::Bit1, verilog::Bit::True));
	assertEquals(state.valueOf(wireId), DataBits(Width::Bit1, verilog::Bit::True));

	// Tick once.
	state.tick();

	// Check.
	std::cout << "[5] pin = " << state.valueOf(pinId) << " wire = " << state.valueOf(wireId) << '\n';
	assertEquals(state.valueOf(pinId), DataBits(Width::Bit1, verilog::Bit::False));
	assertEquals(state.valueOf(wireId), DataBits(Width::Bit1, verilog::Bit::False));

	// Tick once.
	state.tick();

	// Check.
	std::cout << "[6] pin = " << state.valueOf(pinId) << " wire = " << state.valueOf(wireId) << '\n';
	assertEquals(state.valueOf(pinId), DataBits(Width::Bit1, verilog::Bit::False));
	assertEquals(state.valueOf(wireId), DataBits(Width::Bit1, verilog::Bit::False));
}

int main()
{
	test1();
	test2();
}
