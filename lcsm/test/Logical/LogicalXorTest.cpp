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

#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

using namespace lcsm;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

using TestData = std::tuple< DataBits, DataBits, DataBits >;

static LCSMCircuit getLogicalOrNotCircuit()
{
	LCSMCircuit circuit("LogicalOrNot");

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
	circuit.connect(b, n2, model::Transistor::Port::Base);
	circuit.connect(a, p1, model::Transistor::Port::Base);
	circuit.connect(b, p2, model::Transistor::Port::Base);

	circuit.connect(power, p1, model::Transistor::Port::SrcB);
	circuit.connect(ground, n1, model::Transistor::Port::SrcA);
	circuit.connect(ground, n2, model::Transistor::Port::SrcA);

	circuit.connect(p1, model::Transistor::Port::SrcA, p2, model::Transistor::Port::SrcB);

	circuit.connect(p2, model::Transistor::Port::SrcA, c);
	circuit.connect(n1, model::Transistor::Port::SrcB, c);
	circuit.connect(n2, model::Transistor::Port::SrcB, c);

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

static LCSMCircuit getLogicalOrCircuit()
{
	LCSMCircuit circuit("LogicalOr");

	// Create all needed circuit elements.
	model::Pin *a = circuit.createPin(false, "a");
	model::Pin *b = circuit.createPin(false, "b");
	model::Pin *c = circuit.createPin(true, "c");
	LCSMCircuitView viewNot = circuit.addCircuit(getLogicalNotCircuit());
	LCSMCircuitView viewOrNot = circuit.addCircuit(getLogicalOrNotCircuit());

	// Make connections.
	circuit.connect(a, viewOrNot.findInput("a"), model::Pin::Port::External);
	circuit.connect(b, viewOrNot.findInput("b"), model::Pin::Port::External);
	circuit.connect(viewOrNot.findOutput("c"), model::Pin::Port::External, viewNot.findInput("input"), model::Pin::Port::External);
	circuit.connect(viewNot.findOutput("output"), model::Pin::Port::External, c);

	return circuit;
}

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

	model::Wire *w1 = circuit.connect(n2, model::Transistor::Port::SrcB, c);
	w1->setName("w1");

	model::Wire *w2 = circuit.connect(p1, model::Transistor::Port::SrcA, c);
	w2->setName("w2");

	model::Wire *w3 = circuit.connect(p2, model::Transistor::Port::SrcA, c);
	w3->setName("w3");

	return circuit;
}

static LCSMCircuit getLogicalAndCircuit()
{
	LCSMCircuit circuit("LogicalAnd");

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

static LCSMCircuit generator()
{
	LCSMCircuit circuit("LogicalXorTest");

	// Create all needed components.
	model::Pin *a = circuit.createPin(false, "a");
	model::Pin *b = circuit.createPin(false, "b");
	model::Pin *c = circuit.createPin(true, "c");

	LCSMCircuitView nandView = circuit.addCircuit(getLogicalAndNotCircuit());
	nandView.setName("nand");

	LCSMCircuitView orView = circuit.addCircuit(getLogicalOrCircuit());
	orView.setName("or");

	LCSMCircuitView andView = circuit.addCircuit(getLogicalAndCircuit());
	andView.setName("and");

	// Make connections.
	model::Wire *w1 = circuit.connect(a, orView.find("a"), model::Pin::Port::External);
	w1->setName("w1");

	model::Wire *w2 = circuit.connect(b, orView.find("b"), model::Pin::Port::External);
	w2->setName("w2");

	model::Wire *w3 = circuit.connect(a, nandView.find("a"), model::Pin::Port::External);
	w3->setName("w3");

	model::Wire *w4 = circuit.connect(b, nandView.find("b"), model::Pin::Port::External);
	w4->setName("w4");

	model::Wire *w5 = circuit.connect(orView.find("c"), model::Pin::Port::External, andView.find("a"), model::Pin::Port::External);
	w5->setName("w5");

	model::Wire *w6 = circuit.connect(nandView.find("c"), model::Pin::Port::External, andView.find("b"), model::Pin::Port::External);
	w6->setName("w6");

	model::Wire *w7 = circuit.connect(andView.find("c"), model::Pin::Port::External, c);
	w7->setName("w7");

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find components.
	Component *a = circuit.find("a");
	Component *b = circuit.find("b");
	Component *c = circuit.find("c");
	Component *w1 = circuit.find("w1");
	Component *w2 = circuit.find("w2");
	Component *w3 = circuit.find("w3");
	Component *w4 = circuit.find("w4");
	Component *w5 = circuit.find("w5");
	Component *w6 = circuit.find("w6");
	Component *w7 = circuit.find("w7");

	LCSMCircuitView nandView = circuit.findCircuit("nand");
	LCSMCircuitView orView = circuit.findCircuit("or");
	LCSMCircuitView andView = circuit.findCircuit("and");

	// Check types.
	assertType(a, ComponentType::Pin);
	assertType(b, ComponentType::Pin);
	assertType(c, ComponentType::Pin);
	assertType(w1, ComponentType::Wire);
	assertType(w2, ComponentType::Wire);
	assertType(w3, ComponentType::Wire);
	assertType(w4, ComponentType::Wire);
	assertType(w5, ComponentType::Wire);
	assertType(w6, ComponentType::Wire);
	assertType(w7, ComponentType::Wire);

	assertTrue(nandView.present());
	assertTrue(orView.present());
	assertTrue(andView.present());
}

static void test(LCSMCircuit &circuit)
{
	// Find components.
	Component *a = circuit.find("a");
	Component *b = circuit.find("b");
	Component *c = circuit.find("c");

	// Indexes.
	const Identifier aId = a->id();
	const Identifier bId = b->id();
	const Identifier cId = c->id();

	// Testing data.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { F }, { F }, { F } },
		{ { F }, { T }, { T } },
		{ { T }, { F }, { T } },
		{ { T }, { T }, { F } },
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

		// Printout.
		std::cout << "<a = " << aActual << ">, <b = " << bActual << "> --> <c = " << cActual << ">\n";

		assertEquals(aActual, inA);
		assertEquals(bActual, inB);
		assertEquals(cActual, ref);
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
