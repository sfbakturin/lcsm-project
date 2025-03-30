#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <iostream>

using namespace lcsm;
using namespace lcsm::support;
using namespace lcsm::testing;
using namespace lcsm::verilog;

static LCSMCircuit generator()
{
	LCSMCircuit circuit("TunnelTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");
	model::Tunnel *tunnel1 = circuit.createTunnel("tunnel1");
	model::Tunnel *tunnel2 = circuit.createTunnel("tunnel2");

	// Make connections.
	tunnel1->connectTunnel(tunnel2);
	tunnel2->connectTunnel(tunnel1);
	model::Wire *wire1 = circuit.connect(input, tunnel1);
	model::Wire *wire2 = circuit.connect(output, tunnel2);
	wire1->setName("wire1");
	wire2->setName("wire2");

	return circuit;
}

static void checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *tunnel1 = circuit.find("tunnel1");
	Circuit *tunnel2 = circuit.find("tunnel2");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");

	// Check element's types.
	assertType(input, CircuitType::Pin);
	assertType(output, CircuitType::Pin);
	assertType(tunnel1, CircuitType::Tunnel);
	assertType(tunnel2, CircuitType::Tunnel);
	assertType(wire1, CircuitType::Wire);
	assertType(wire2, CircuitType::Wire);
}

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *tunnel1 = circuit.find("tunnel1");
	Circuit *tunnel2 = circuit.find("tunnel2");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");

	// Extract models.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Pin *outputModel = static_cast< model::Pin * >(output);

	// Indexes.
	const Identifier inputId = input->id();
	const Identifier outputId = output->id();
	const Identifier tunnel1Id = tunnel1->id();
	const Identifier tunnel2Id = tunnel2->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();

	// Test!
	for (Width width : Widths)
	{
		// Change model's settings.
		inputModel->setWidth(width);
		outputModel->setWidth(width);

		// Generate physical engine.
		LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

		// Fork new state.
		LCSMState state = engine.fork();

		// Single test.
		for (Bit bit : Bits)
		{
			// Testing data.
			const DataBits testdata{ width, bit };

			// Put value.
			state.putValue(inputId, testdata);

			// Step once.
			state.tick();

			// Assert.
			const DataBits &inputActual = state.valueOf(inputId);
			const DataBits &outputActual = state.valueOf(outputId);
			const DataBits &tunnel1Actual = state.valueOf(tunnel1Id);
			const DataBits &tunnel2Actual = state.valueOf(tunnel2Id);
			const DataBits &wire1Actual = state.valueOf(wire1Id);
			const DataBits &wire2Actual = state.valueOf(wire2Id);
			assertEquals(inputActual, testdata);
			assertEquals(outputActual, testdata);
			assertEquals(tunnel1Actual, testdata);
			assertEquals(tunnel2Actual, testdata);
			assertEquals(wire1Actual, testdata);
			assertEquals(wire2Actual, testdata);

			// Printout.
			std::cout << "<pin = " << inputActual << "> --> <tunnel = " << tunnel1Actual
					  << "> --> <tunnel = " << tunnel2Actual << "> --> <pin = " << outputActual << ">\n";
		}
	}
}

int main()
{
	PerformTest(generator, checker, test);
}
