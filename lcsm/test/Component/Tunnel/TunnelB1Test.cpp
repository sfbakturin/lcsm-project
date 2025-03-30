#include "TunnelBTesting.h"
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

using namespace lcsm;
using namespace lcsm::support;
using namespace lcsm::testing;
using namespace lcsm::verilog;
using namespace lcsm_tests;

static void test(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output1 = circuit.find("output1");
	Circuit *output2 = circuit.find("output2");
	Circuit *output3 = circuit.find("output3");
	Circuit *tunnel1 = circuit.find("tunnel1");
	Circuit *tunnel2 = circuit.find("tunnel2");
	Circuit *tunnel3 = circuit.find("tunnel3");
	Circuit *tunnel4 = circuit.find("tunnel4");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");
	Circuit *wire3 = circuit.find("wire3");
	Circuit *wire4 = circuit.find("wire4");

	// Extract models.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Pin *output1Model = static_cast< model::Pin * >(output1);
	model::Pin *output2Model = static_cast< model::Pin * >(output2);
	model::Pin *output3Model = static_cast< model::Pin * >(output3);

	// Indexes.
	const Identifier inputId = input->id();
	const Identifier output1Id = output1->id();
	const Identifier output2Id = output2->id();
	const Identifier output3Id = output3->id();
	const Identifier tunnel1Id = tunnel1->id();
	const Identifier tunnel2Id = tunnel2->id();
	const Identifier tunnel3Id = tunnel3->id();
	const Identifier tunnel4Id = tunnel4->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();
	const Identifier wire3Id = wire3->id();
	const Identifier wire4Id = wire4->id();

	// Test!
	for (Width width : Widths)
	{
		// Change model's settings.
		inputModel->setWidth(width);
		output1Model->setWidth(width);
		output2Model->setWidth(width);
		output3Model->setWidth(width);

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
			const DataBits &output1Actual = state.valueOf(output1Id);
			const DataBits &output2Actual = state.valueOf(output2Id);
			const DataBits &output3Actual = state.valueOf(output3Id);
			const DataBits &tunnel1Actual = state.valueOf(tunnel1Id);
			const DataBits &tunnel2Actual = state.valueOf(tunnel2Id);
			const DataBits &tunnel3Actual = state.valueOf(tunnel3Id);
			const DataBits &tunnel4Actual = state.valueOf(tunnel4Id);
			const DataBits &wire1Actual = state.valueOf(wire1Id);
			const DataBits &wire2Actual = state.valueOf(wire2Id);
			const DataBits &wire3Actual = state.valueOf(wire3Id);
			const DataBits &wire4Actual = state.valueOf(wire4Id);
			assertEquals(inputActual, testdata);
			assertEquals(output1Actual, testdata);
			assertEquals(output2Actual, testdata);
			assertEquals(output3Actual, testdata);
			assertEquals(tunnel1Actual, testdata);
			assertEquals(tunnel2Actual, testdata);
			assertEquals(tunnel3Actual, testdata);
			assertEquals(tunnel4Actual, testdata);
			assertEquals(wire1Actual, testdata);
			assertEquals(wire2Actual, testdata);
			assertEquals(wire3Actual, testdata);
			assertEquals(wire4Actual, testdata);
		}
	}
}

int main()
{
	// Run all tests.
	PerformTest(generator, checker, test);
}
