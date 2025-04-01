#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Button.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

// Constants.
static const DataBits T = { Width::Bit1, Bit::True };
static const DataBits F = { Width::Bit1, Bit::False };

static LCSMCircuit generator()
{
	LCSMCircuit circuit("ButtonCircuitTest");

	// Create all needed circuit elements.
	model::Button *button = circuit.createButton("button");
	model::Pin *pin = circuit.createPin(true, "pin");

	// Make connections.
	model::Wire *wire = circuit.connect(button, pin);
	wire->setName("wire");

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *button = circuit.find("button");
	Component *pin = circuit.find("pin");
	Component *wire = circuit.find("wire");

	// Check element's types.
	assertType(button, ComponentType::Button);
	assertType(pin, ComponentType::Pin);
	assertType(wire, ComponentType::Wire);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *button = circuit.find("button");
	Component *pin = circuit.find("pin");

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Indexes.
	const Identifier buttonId = button->id();
	const Identifier pinId = pin->id();

	// Single test 1. When we do not push button.
	{
		// Tick.
		state.tick();

		// Assertion.
		const DataBits &expected = F;
		const DataBits &actualButton = state.valueOf(buttonId);
		const DataBits &actualPin = state.valueOf(pinId);
		assertEquals(actualButton, expected);
		assertEquals(actualPin, expected);
	}

	// Single test 2. When we do start pushing button.
	{
		// Expected.
		const DataBits &expected = T;

		// Push the button: place 1bit true to button.
		state.putValue(buttonId, T);

		// Tick.
		state.tick();

		// Assertion.
		const DataBits &actualButton = state.valueOf(buttonId);
		const DataBits &actualPin = state.valueOf(pinId);
		assertEquals(actualButton, expected);
		assertEquals(actualPin, expected);
	}

	// Single test 3. When we do stop pushing button.
	{
		// Expected.
		const DataBits &expected = F;

		// Stop push the button: place 1bit false to button.
		state.putValue(button->id(), F);

		// Tick.
		state.tick();

		// Assertion.
		const DataBits &actualButton = state.valueOf(button->id());
		const DataBits &actualPin = state.valueOf(pin->id());
		assertEquals(actualButton, expected);
		assertEquals(actualPin, expected);
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
