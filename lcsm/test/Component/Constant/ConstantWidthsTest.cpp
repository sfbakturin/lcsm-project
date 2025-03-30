#include "ConstantTesting.h"
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

#include <iostream>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm_tests;

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *constant = circuit.find("constant");
	Circuit *pin = circuit.find("pin");
	Circuit *wire = circuit.find("wire");

	// Extract models.
	model::Constant *constantModel = static_cast< model::Constant * >(constant);
	model::Pin *pinModel = static_cast< model::Pin * >(pin);

	// Indexes.
	const Identifier constantId = constant->id();
	const Identifier pinId = pin->id();
	const Identifier wireId = wire->id();

	// Test!
	for (Width width : Widths)
	{
		// Change model's settings.
		constantModel->setWidth(width);
		pinModel->setWidth(width);

		// Generate physical engine.
		LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

		// Fork new state.
		LCSMState state = engine.fork();

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
}

int main()
{
	PerformTest(generator, checker, test);
}
