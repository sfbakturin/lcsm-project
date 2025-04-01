#include "ConstantTesting.h"
#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm_tests;

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *constant = circuit.find("constant");
	Component *pin = circuit.find("pin");
	Component *wire = circuit.find("wire");

	// Extract models.
	model::Constant *constantModel = static_cast< model::Constant * >(constant);
	model::Pin *pinModel = static_cast< model::Pin * >(pin);

	// Indexes.
	const Identifier constantId = constant->id();
	const Identifier pinId = pin->id();
	const Identifier wireId = wire->id();

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
}

int main()
{
	PerformTest(generator, checker, test);
}
