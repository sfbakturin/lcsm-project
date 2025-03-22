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

	circuit.connect(n2, model::Transistor::Port::SrcB, c);
	circuit.connect(p1, model::Transistor::Port::SrcA, c);
	circuit.connect(p2, model::Transistor::Port::SrcA, c);

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *a = circuit.find("a");
	Circuit *b = circuit.find("b");
	Circuit *c = circuit.find("c");
	Circuit *power = circuit.find("power");
	Circuit *ground = circuit.find("ground");
	Circuit *p1 = circuit.find("p1");
	Circuit *p2 = circuit.find("p2");
	Circuit *n1 = circuit.find("n1");
	Circuit *n2 = circuit.find("n2");

	// Check element's types.
	assertType(a, CircuitType::Pin);
	assertType(b, CircuitType::Pin);
	assertType(c, CircuitType::Pin);
	assertType(power, CircuitType::Power);
	assertType(ground, CircuitType::Ground);
	assertType(p1, CircuitType::Transistor);
	assertType(p2, CircuitType::Transistor);
	assertType(n1, CircuitType::Transistor);
	assertType(n2, CircuitType::Transistor);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *a = circuit.find("a");
	Circuit *b = circuit.find("b");
	Circuit *c = circuit.find("c");
	Circuit *n1A = circuit.find("n1");
	Circuit *n2B = circuit.find("n2");

	// Indexes.
	const Identifier aId = a->id();
	const Identifier bId = b->id();
	const Identifier cId = c->id();
	const Identifier n1AId = n1A->id();
	const Identifier n2BId = n2B->id();

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
		assertEquals(aActual, inA);
		assertEquals(bActual, inB);
		assertEquals(cActual, ref);

		// Printout.
		std::cout << "<a = " << aActual << ">, <b = " << bActual << "> --> <c = " << cActual << ">\n";
		std::cout << "<n1[base = " << transistorABase << ", srcA = " << transistorASrcA << ", srcB = " << transistorASrcB << "]\n";
		std::cout << "<n2[base = " << transistorBBase << ", srcA = " << transistorBSrcA << ", srcB = " << transistorBSrcB << "]\n";
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
