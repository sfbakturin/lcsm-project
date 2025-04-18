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

static LCSMCircuit generator()
{
	LCSMCircuit circuit("LogicalOrTest");

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

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *a = circuit.findInput("a");
	Component *b = circuit.findInput("b");
	Component *c = circuit.findOutput("c");
	LCSMCircuitView viewOrNot = circuit.findCircuit("LogicalOrNot");
	LCSMCircuitView viewNot = circuit.findCircuit("LogicalNot");

	// Check element's types.
	assertType(a, ComponentType::Pin);
	assertType(b, ComponentType::Pin);
	assertType(c, ComponentType::Pin);

	assertTrue(viewOrNot.present());
	assertTrue(viewNot.present());

	Component *aViewOrNot = viewOrNot.findInput("a");
	Component *bViewOrNot = viewOrNot.findInput("b");
	Component *cViewOrNot = viewOrNot.findOutput("c");
	Component *inputViewNot = viewNot.findInput("input");
	Component *outputViewNot = viewNot.findOutput("output");

	assertType(aViewOrNot, ComponentType::Pin);
	assertType(bViewOrNot, ComponentType::Pin);
	assertType(cViewOrNot, ComponentType::Pin);
	assertType(inputViewNot, ComponentType::Pin);
	assertType(outputViewNot, ComponentType::Pin);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *a = circuit.find("a");
	Component *b = circuit.find("b");
	Component *c = circuit.find("c");
	LCSMCircuitView viewOrNot = circuit.findCircuit("LogicalOrNot");
	Component *aViewOrNot = viewOrNot.findInput("a");
	Component *bViewOrNot = viewOrNot.findInput("b");
	Component *n1A = viewOrNot.find("n1");
	Component *n2B = viewOrNot.find("n2");
	Component *p1A = viewOrNot.find("p1");
	Component *p2B = viewOrNot.find("p2");

	// Indexes.
	const Identifier aId = a->id();
	const Identifier bId = b->id();
	const Identifier cId = c->id();
	const Identifier aViewAndNotId = aViewOrNot->id();
	const Identifier bViewAndNotId = bViewOrNot->id();
	const Identifier n1AId = n1A->id();
	const Identifier n2BId = n2B->id();
	const Identifier p1AId = p1A->id();
	const Identifier p2BId = p2B->id();

	// Testing data.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { F }, { F }, { F } },
		{ { F }, { T }, { T } },
		{ { T }, { F }, { T } },
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
		const DataBits &transistorABase = state.valueOf(n1AId, 0);
		const DataBits &transistorASrcA = state.valueOf(n1AId, 1);
		const DataBits &transistorASrcB = state.valueOf(n1AId, 2);
		const DataBits &transistorBBase = state.valueOf(n2BId, 0);
		const DataBits &transistorBSrcA = state.valueOf(n2BId, 1);
		const DataBits &transistorBSrcB = state.valueOf(n2BId, 2);
		const DataBits &transistor2ABase = state.valueOf(p1AId, 0);
		const DataBits &transistor2ASrcA = state.valueOf(p1AId, 1);
		const DataBits &transistor2ASrcB = state.valueOf(p1AId, 2);
		const DataBits &transistor2BBase = state.valueOf(p2BId, 0);
		const DataBits &transistor2BSrcA = state.valueOf(p2BId, 1);
		const DataBits &transistor2BSrcB = state.valueOf(p2BId, 2);

		// Printout.
		std::cout << "<a = " << aActual << ">, <b = " << bActual << "> --> <c = " << cActual << ">\n";
		std::cout << "<n1[base = " << transistorABase << ", srcA = " << transistorASrcA << ", srcB = " << transistorASrcB << "]\n";
		std::cout << "<n2[base = " << transistorBBase << ", srcA = " << transistorBSrcA << ", srcB = " << transistorBSrcB << "]\n";
		std::cout << "<p1[base = " << transistor2ABase << ", srcA = " << transistor2ASrcA << ", srcB = " << transistor2ASrcB << "]\n";
		std::cout << "<p2[base = " << transistor2BBase << ", srcA = " << transistor2BSrcA << ", srcB = " << transistor2BSrcB << "]\n";

		assertEquals(aActual, inA);
		assertEquals(bActual, inB);
		assertEquals(aViewOrNotActual, inA);
		assertEquals(bViewOrNotActual, inB);
		assertEquals(cActual, ref);
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
