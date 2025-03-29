#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
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

using TestData = std::tuple< DataBits, DataBits, DataBits, DataBits >;

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

static LCSMCircuit getLogicalOr()
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

static LCSMCircuit generator()
{
	LCSMCircuit circuit("RSTrigger");

	// Create all needed circuit elements.
	model::Pin *r = circuit.createPin(false, "R");
	model::Pin *s = circuit.createPin(false, "S");
	model::Pin *q = circuit.createPin(true, "Q");
	model::Pin *nq = circuit.createPin(true, "nQ");
	LCSMCircuitView or1 = circuit.addCircuit(getLogicalOr());
	or1.setName("or1");
	LCSMCircuitView or2 = circuit.addCircuit(getLogicalOr());
	or2.setName("or2");

	// Make connections.
	circuit.connect(r, or1.find("a"), model::Pin::Port::External);
	circuit.connect(s, or2.find("b"), model::Pin::Port::External);

	model::Wire *w1 = circuit.connect(or1.find("c"), model::Pin::Port::External, q);
	model::Wire *w2 = circuit.connect(or2.find("c"), model::Pin::Port::External, nq);

	circuit.connect(w1, or2.find("a"), model::Pin::Port::External);
	circuit.connect(w2, or1.find("b"), model::Pin::Port::External);

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *r = circuit.find("R");
	Circuit *s = circuit.find("S");
	Circuit *q = circuit.find("Q");
	Circuit *nq = circuit.find("nQ");
	LCSMCircuitView or1 = circuit.findCircuit("or1");
	LCSMCircuitView or2 = circuit.findCircuit("or2");

	// Check element's types.
	assertType(r, CircuitType::Pin);
	assertType(s, CircuitType::Pin);
	assertType(q, CircuitType::Pin);
	assertType(nq, CircuitType::Pin);

	assertTrue(or1.present());
	assertTrue(or2.present());
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *r = circuit.find("R");
	Circuit *s = circuit.find("S");
	Circuit *q = circuit.find("Q");
	Circuit *nq = circuit.find("nQ");

	// Indexes.
	const Identifier rId = r->id();
	const Identifier sId = s->id();
	const Identifier qId = q->id();
	const Identifier nqId = nq->id();

	// Testing data.
	// clang-format off
	const std::vector< TestData > testdatas = {
		{ { F }, { F }, { F }, { F } },
		// { { F }, { T }, { F }, { F } },
		// { { F }, { F }, { F }, { F } },
		{ { F }, { F }, { F }, { F } },
		{ { T }, { F }, { F }, { F } },
		{ { T }, { F }, { F }, { F } },
		{ { F }, { F }, { F }, { F } },
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
		const DataBits &inR = std::get< 0 >(testdata);
		const DataBits &inS = std::get< 1 >(testdata);
		// const DataBits &refQ = std::get< 2 >(testdata);
		// const DataBits &refnQ = std::get< 2 >(testdata);

		// Put value.
		state.putValue(rId, inR);
		state.putValue(sId, inS);

		// Step once.
		state.tick();

		// Assert.
		const DataBits &qActual = state.valueOf(qId);
		const DataBits &nqActual = state.valueOf(nqId);

		// Printout.
		std::cout
			<< "R = " << inR << ", S = " << inS
			<< " ===> "
			   "Q = "
			<< qActual << ", nQ = " << nqActual << "\n";
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
