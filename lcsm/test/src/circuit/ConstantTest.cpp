#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <array>
#include <iostream>
#include <tuple>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

using TestData = std::tuple< Width, value_t, DataBits >;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

static LCSMCircuit generator()
{
	LCSMCircuit circuit;
	model::Constant *constant = circuit.createConstant("constant");
	model::Pin *pin = circuit.createPin(true, "pin");
	circuit.connect(constant, pin, "wire");
	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *constant = circuit.find("constant");
	Circuit *pin = circuit.find("pin");
	Circuit *wire = circuit.find("wire");

	// Check element's types.
	assertType(constant, CircuitType::Constant);
	assertType(pin, CircuitType::Pin);
	assertType(wire, CircuitType::Wire);
}

static void test0_pretest()
{
	const GeneratorTy g = generator;
	const CheckerTy c = checker;
	preTest(g, c);
}

static void test1(LCSMState &state, const Circuit *constant, const Circuit *pin, const Circuit *wire, Width width)
{
	// Indexes.
	const Identifier constantId = constant->id();
	const Identifier pinId = pin->id();
	const Identifier wireId = wire->id();

	// Step once.
	state.tick();

	// Assert.
	const DataBits expected{ width, Bit::True };
	const DataBits &actualConstant = state.valueOf(constantId);
	const DataBits &actualPin = state.valueOf(pinId);
	const DataBits &actualWire = state.valueOf(wireId);
	assertEquals(actualConstant, expected);
	assertEquals(actualPin, expected);
	assertEquals(actualWire, expected);
	std::cout << "<constant=" << actualConstant << "> --> <wire=" << actualWire << "> --> <pin=" << actualPin << ">\n";
}

static void test1_widths()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *constant = circuit.find("constant");
	Circuit *pin = circuit.find("pin");
	Circuit *wire = circuit.find("wire");

	// Extract models.
	model::Constant *constantModel = static_cast< model::Constant * >(constant);
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
		constantModel->setWidth(width);
		pinModel->setWidth(width);

		// Generate physical engine.
		LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

		// Fork new state.
		LCSMState state = engine.fork();

		// Run single test.
		test1(state, constant, pin, wire, width);
	}
}

static void test2(LCSMState &state, const Circuit *constant, const Circuit *pin, const Circuit *wire, const DataBits &expected)
{
	// Indexes.
	const Identifier constantId = constant->id();
	const Identifier pinId = pin->id();
	const Identifier wireId = wire->id();

	// Step once.
	state.tick();

	// Assert.
	const DataBits &actualConstant = state.valueOf(constantId);
	const DataBits &actualPin = state.valueOf(pinId);
	const DataBits &actualWire = state.valueOf(wireId);
	assertEquals(actualConstant, expected);
	assertEquals(actualPin, expected);
	assertEquals(actualWire, expected);
	std::cout << "<constant=" << actualConstant << "> --> <wire=" << actualWire << "> --> <pin=" << actualPin << ">\n";
}

static void test2_values()
{
	LCSMCircuit circuit = generator();

	// Find circuits.
	Circuit *constant = circuit.find("constant");
	Circuit *pin = circuit.find("pin");
	Circuit *wire = circuit.find("wire");

	// Extract models.
	model::Constant *constantModel = static_cast< model::Constant * >(constant);
	model::Pin *pinModel = static_cast< model::Pin * >(pin);

	// All widths and values.
	// clang-format off
    const std::vector< TestData > testdatas = {
        { Width::Bit1, 0x0,  { F } },
        { Width::Bit2, 0x2,  { T, F } },
        { Width::Bit3, 0x5,  { T, F, T } },
        { Width::Bit3, 0x6,  { T, T, F } },
        { Width::Bit4, 0xE,  { T, T, T, F } },
        { Width::Bit4, 0x6,  { F, T, T, F } },
        { Width::Byte, 0xCC, { T, T, F, F, T, T, F, F } }
    };
	// clang-format on

	// Test!
	for (const TestData &testdata : testdatas)
	{
		// Test's data.
		const Width width = std::get< 0 >(testdata);
		const value_t value = std::get< 1 >(testdata);
		const DataBits &expected = std::get< 2 >(testdata);

		// Change model's settings.
		constantModel->setWidth(width);
		constantModel->setValue(value);
		pinModel->setWidth(width);

		// Generate physical engine.
		LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

		// Fork new state.
		LCSMState state = engine.fork();

		// Run single test.
		test2(state, constant, pin, wire, expected);
	}
}

int main()
{
	test0_pretest();
	test1_widths();
	test2_values();
}
