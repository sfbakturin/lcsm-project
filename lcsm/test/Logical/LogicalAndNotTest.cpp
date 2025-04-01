#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
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

static LCSMCircuit generator()
{
	LCSMCircuit circuit("LogicalAndNotTest");

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

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *a = circuit.find("a");
	Component *b = circuit.find("b");
	Component *c = circuit.find("c");
	Component *power = circuit.find("power");
	Component *ground = circuit.find("ground");
	Component *p1 = circuit.find("p1");
	Component *p2 = circuit.find("p2");
	Component *n1 = circuit.find("n1");
	Component *n2 = circuit.find("n2");

	// Check element's types.
	assertType(a, ComponentType::Pin);
	assertType(b, ComponentType::Pin);
	assertType(c, ComponentType::Pin);
	assertType(power, ComponentType::Power);
	assertType(ground, ComponentType::Ground);
	assertType(p1, ComponentType::Transistor);
	assertType(p2, ComponentType::Transistor);
	assertType(n1, ComponentType::Transistor);
	assertType(n2, ComponentType::Transistor);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *a = circuit.find("a");
	Component *b = circuit.find("b");
	Component *c = circuit.find("c");
	Component *n1A = circuit.find("n1");
	Component *n2B = circuit.find("n2");
	Component *p1A = circuit.find("p1");
	Component *p2B = circuit.find("p2");
	Component *w1 = circuit.find("w1");
	Component *w2 = circuit.find("w2");
	Component *w3 = circuit.find("w3");

	// Indexes.
	const Identifier aId = a->id();
	const Identifier bId = b->id();
	const Identifier cId = c->id();
	const Identifier n1AId = n1A->id();
	const Identifier n2BId = n2B->id();
	const Identifier p1AId = p1A->id();
	const Identifier p2BId = p2B->id();
	const Identifier w1Id = w1->id();
	const Identifier w2Id = w2->id();
	const Identifier w3Id = w3->id();

	// Testing data.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { F }, { F }, { T } },
		{ { F }, { T }, { T } },
		{ { T }, { F }, { T } },
		{ { T }, { T }, { F } }
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

		const DataBits &w1Actual = state.valueOf(w1Id);
		const DataBits &w2Actual = state.valueOf(w2Id);
		const DataBits &w3Actual = state.valueOf(w3Id);

		// Printout.
		std::cout << "<a = " << aActual << ">, <b = " << bActual << "> --> <c = " << cActual << ">\n";
		std::cout << "<n1[base = " << transistorABase << ", srcA = " << transistorASrcA << ", srcB = " << transistorASrcB << "]\n";
		std::cout << "<n2[base = " << transistorBBase << ", srcA = " << transistorBSrcA << ", srcB = " << transistorBSrcB << "]\n";
		std::cout << "<p1[base = " << transistor2ABase << ", srcA = " << transistor2ASrcA << ", srcB = " << transistor2ASrcB << "]\n";
		std::cout << "<p2[base = " << transistor2BBase << ", srcA = " << transistor2BSrcA << ", srcB = " << transistor2BSrcB << "]\n";
		std::cout << "<w1 = " << w1Actual << ">\n";
		std::cout << "<w2 = " << w2Actual << ">\n";
		std::cout << "<w3 = " << w3Actual << ">\n";

		assertEquals(aActual, inA);
		assertEquals(bActual, inB);
		assertEquals(cActual, ref);
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
