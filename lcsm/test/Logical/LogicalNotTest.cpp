#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

using TestData = std::pair< DataBits, DataBits >;

static LCSMCircuit generator()
{
	LCSMCircuit circuit("LogicalNotTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");
	model::Power *power = circuit.createPower("power");
	model::Ground *ground = circuit.createGround("ground");
	model::Transistor *transistorP = circuit.createTransistor("transistorP", model::Transistor::Type::P);
	model::Transistor *transistorN = circuit.createTransistor("transistorN", model::Transistor::Type::N);

	// Make connections.
	circuit.connect(input, transistorP, model::Transistor::Port::Base);
	circuit.connect(input, transistorN, model::Transistor::Port::Base);
	circuit.connect(power, transistorP, model::Transistor::Port::SrcB);
	circuit.connect(ground, transistorN, model::Transistor::Port::SrcA);
	circuit.connect(transistorP, model::Transistor::Port::SrcA, output);
	circuit.connect(transistorN, model::Transistor::Port::SrcB, output);

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *power = circuit.find("power");
	Circuit *ground = circuit.find("ground");
	Circuit *transistorP = circuit.find("transistorP");
	Circuit *transistorN = circuit.find("transistorN");

	// Check element's types.
	assertType(input, CircuitType::Pin);
	assertType(output, CircuitType::Pin);
	assertType(power, CircuitType::Power);
	assertType(ground, CircuitType::Ground);
	assertType(transistorP, CircuitType::Transistor);
	assertType(transistorN, CircuitType::Transistor);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	// Circuit *power = circuit.find("power");
	// Circuit *ground = circuit.find("ground");
	// Circuit *transistorP = circuit.find("transistorP");
	// Circuit *transistorN = circuit.find("transistorN");

	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	// const Identifier powerId = power->id();
	// const Identifier groundId = ground->id();
	// const Identifier transistorPId = transistorP->id();
	// const Identifier transistorNId = transistorN->id();

	// Testing data.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { T }, { F } },
		{ { T }, { F } },
		{ { T }, { F } },
		{ { F }, { T } },
		{ { F }, { T } },
		{ { F }, { T } },
		{ { F }, { T } },
		{ { T }, { F } },
		{ { F }, { T } },
	};
	// clang-format on

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (const TestData &testdata : testdatas)
	{
		// Testing data.
		const DataBits &in = testdata.first;
		const DataBits &ref = testdata.second;

		// Put value.
		state.putValue(inputId, in);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &inputActual = state.valueOf(inputId);
		const DataBits &outputActual = state.valueOf(outputId);
		assertEquals(inputActual, in);
		assertEquals(outputActual, ref);

		// Printout.
		std::cout << "<input = " << inputActual << "> --> <input0 = "
				  << "output = " << outputActual << ">\n";
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
