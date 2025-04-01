#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
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
#include <tuple>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

using TestData = std::tuple< DataBits, DataBits, DataBits >;

static LCSMCircuit getLogicalAndNotCircuit()
{
	LCSMCircuit circuit("LogicalAndNot");

	// Create all needed circuit elements.
	model::Pin *a = circuit.createPin(false, "a");
	model::Pin *b = circuit.createPin(false, "b");
	model::Pin *c = circuit.createPin(true, "c");
	model::Power *power = circuit.createPower("power");
	model::Ground *ground = circuit.createGround("ground");
	model::Transistor *p1 = circuit.createTransistor("p1", model::Transistor::Type::P);
	model::Transistor *p2 = circuit.createTransistor("p2", model::Transistor::Type::P);
	model::Transistor *n1 = circuit.createTransistor("n1", model::Transistor::Type::N);
	model::Transistor *n2 = circuit.createTransistor("n2", model::Transistor::Type::N);

	// Make connections.
	circuit.connect(a, n1, model::Transistor::Port::Base);
	circuit.connect(a, p1, model::Transistor::Port::Base);

	circuit.connect(b, n2, model::Transistor::Port::Base);
	circuit.connect(b, p2, model::Transistor::Port::Base);

	circuit.connect(ground, n1, model::Transistor::Port::SrcA);

	circuit.connect(power, p1, model::Transistor::Port::SrcB);
	circuit.connect(power, p2, model::Transistor::Port::SrcB);

	circuit.connect(n1, model::Transistor::Port::SrcB, n2, model::Transistor::Port::SrcA);

	circuit.connect(n2, model::Transistor::Port::SrcB, c);
	circuit.connect(p1, model::Transistor::Port::SrcA, c);
	circuit.connect(p2, model::Transistor::Port::SrcA, c);

	return circuit;
}

static LCSMCircuit getLogicalNotCircuit()
{
	LCSMCircuit circuit("LogicalNot");

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

static LCSMCircuit generator()
{
	LCSMCircuit circuit("LogicalAndTest");

	// Create all needed circuit elements.
	model::Pin *a = circuit.createPin(false, "a");
	model::Pin *b = circuit.createPin(false, "b");
	model::Pin *c = circuit.createPin(true, "c");
	LCSMCircuitView viewNot = circuit.addCircuit(getLogicalNotCircuit());
	LCSMCircuitView viewOrNot = circuit.addCircuit(getLogicalAndNotCircuit());

	// Make connections.
	circuit.connect(a, viewOrNot.findInput("a"), model::Pin::Port::External);
	circuit.connect(b, viewOrNot.findInput("b"), model::Pin::Port::External);
	circuit.connect(viewOrNot.findOutput("c"), model::Pin::Port::External, viewNot.findInput("input"), model::Pin::Port::External);
	circuit.connect(viewNot.findOutput("output"), model::Pin::Port::External, c);

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *a = circuit.findInput("a");
	Component *b = circuit.findInput("b");
	Component *c = circuit.findOutput("c");
	LCSMCircuitView viewAndNot = circuit.findCircuit("LogicalAndNot");
	LCSMCircuitView viewNot = circuit.findCircuit("LogicalNot");

	// Check element's types.
	assertType(a, ComponentType::Pin);
	assertType(b, ComponentType::Pin);
	assertType(c, ComponentType::Pin);

	assertTrue(viewAndNot.present());
	assertTrue(viewNot.present());

	Component *aViewAndNot = viewAndNot.findInput("a");
	Component *bViewAndNot = viewAndNot.findInput("b");
	Component *cViewAndNot = viewAndNot.findOutput("c");
	Component *inputViewNot = viewNot.findInput("input");
	Component *outputViewNot = viewNot.findOutput("output");

	assertType(aViewAndNot, ComponentType::Pin);
	assertType(bViewAndNot, ComponentType::Pin);
	assertType(cViewAndNot, ComponentType::Pin);
	assertType(inputViewNot, ComponentType::Pin);
	assertType(outputViewNot, ComponentType::Pin);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *a = circuit.find("a");
	Component *b = circuit.find("b");
	Component *c = circuit.find("c");
	LCSMCircuitView viewAndNot = circuit.findCircuit("LogicalAndNot");
	Component *aViewAndNot = viewAndNot.findInput("a");
	Component *bViewAndNot = viewAndNot.findInput("b");

	// Indexes.
	const Identifier aId = a->id();
	const Identifier bId = b->id();
	const Identifier cId = c->id();
	const Identifier aViewAndNotId = aViewAndNot->id();
	const Identifier bViewAndNotId = bViewAndNot->id();

	// Testing data.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { F }, { F }, { F } },
		{ { F }, { T }, { F } },
		{ { T }, { F }, { F } },
		{ { T }, { T }, { T } },
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
		const DataBits &inA = std::get< 0 >(testdata);
		const DataBits &inB = std::get< 1 >(testdata);
		const DataBits &ref = std::get< 2 >(testdata);

		// Put value.
		state.putValue(aId, inA);
		state.putValue(bId, inB);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &aActual = state.valueOf(aId);
		const DataBits &bActual = state.valueOf(bId);
		const DataBits &cActual = state.valueOf(cId);
		const DataBits &aViewOrNotActual = state.valueOf(aViewAndNotId);
		const DataBits &bViewOrNotActual = state.valueOf(bViewAndNotId);

		assertEquals(aActual, inA);
		assertEquals(bActual, inB);
		assertEquals(aViewOrNotActual, inA);
		assertEquals(bViewOrNotActual, inB);
		assertEquals(cActual, ref);

		// Printout.
		std::cout << "<a = " << aActual << ">, <b = " << bActual << "> --> <c = " << cActual << ">\n";
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
