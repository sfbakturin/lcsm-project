#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <array>
#include <iostream>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static LCSMCircuit generator()
{
	LCSMCircuit circuit;
	model::Power *power = circuit.createPower("power");
	model::Pin *pin = circuit.createPin(true, "pin");
	circuit.connect(power, pin, "wire");
	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *power = circuit.find("power");
	Circuit *pin = circuit.find("pin");
	Circuit *wire = circuit.find("wire");

	// Check element's types.
	assertType(power, CircuitType::Power);
	assertType(pin, CircuitType::Pin);
	assertType(wire, CircuitType::Wire);
}

static void test0_pretest()
{
	const GeneratorTy g = generator;
	const CheckerTy c = checker;
	preTest(g, c);
}

static void test(LCSMState &state, const Circuit *power, const Circuit *pin, const Circuit *wire, Width width)
{
	// Indexes.
	const Identifier powerId = power->id();
	const Identifier pinId = pin->id();
	const Identifier wireId = wire->id();

	// Step once.
	state.tick();

	// Assert.
	const DataBits expected{ width, Bit::True };
	const DataBits &actualPower = state.valueOf(powerId);
	const DataBits &actualPin = state.valueOf(pinId);
	const DataBits &actualWire = state.valueOf(wireId);
	assertEquals(actualPower, expected);
	assertEquals(actualPin, expected);
	assertEquals(actualWire, expected);
	std::cout << "<power=" << actualPower << "> --> <wire=" << actualWire << "> --> <pin=" << actualPin << ">\n";
}

static void test1_widths()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *power = circuit.find("power");
	Circuit *pin = circuit.find("pin");
	Circuit *wire = circuit.find("wire");

	// Extract models.
	model::Power *powerModel = static_cast< model::Power * >(power);
	model::Pin *pinModel = static_cast< model::Pin * >(pin);

	// All widths.
	// clang-format off
	const std::array< Width, 11 > widths = {
		Width::Bit1, Width::Bit2, Width::Bit3,
		Width::Bit4, Width::Bit5, Width::Bit6,
		Width::Bit7, Width::Byte, Width::ShortWord,
		Width::DoubleWord, Width::QuadWord
	};
	// clang-format on

	// Test!
	for (Width width : widths)
	{
		// Change model's settings.
		powerModel->setWidth(width);
		pinModel->setWidth(width);

		// Generate physical engine.
		LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

		// Fork new state.
		LCSMState state = engine.fork();

		// Run single test.
		test(state, power, pin, wire, width);
	}
}

int main()
{
	test0_pretest();
	test1_widths();
}
