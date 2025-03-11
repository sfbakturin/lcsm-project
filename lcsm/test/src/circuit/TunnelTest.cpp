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

static LCSMCircuit generator1()
{
	LCSMCircuit circuit("TunnelCircuitTest");

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

static LCSMCircuit generator2()
{
	LCSMCircuit circuit("TunnelCircuitTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output1 = circuit.createPin(true, "output1");
	model::Pin *output2 = circuit.createPin(true, "output2");
	model::Pin *output3 = circuit.createPin(true, "output3");
	model::Tunnel *tunnel1 = circuit.createTunnel("tunnel1");
	model::Tunnel *tunnel2 = circuit.createTunnel("tunnel2");
	model::Tunnel *tunnel3 = circuit.createTunnel("tunnel3");
	model::Tunnel *tunnel4 = circuit.createTunnel("tunnel4");

	// Make connections.
	tunnel1->connectTunnel(tunnel2);
	tunnel1->connectTunnel(tunnel3);
	tunnel1->connectTunnel(tunnel4);

	tunnel2->connectTunnel(tunnel1);
	tunnel3->connectTunnel(tunnel1);
	tunnel4->connectTunnel(tunnel1);

	model::Wire *wire1 = circuit.connect(input, tunnel1);
	model::Wire *wire2 = circuit.connect(tunnel2, output1);
	model::Wire *wire3 = circuit.connect(tunnel3, output2);
	model::Wire *wire4 = circuit.connect(tunnel4, output3);
	wire1->setName("wire1");
	wire2->setName("wire2");
	wire3->setName("wire3");
	wire4->setName("wire4");

	return circuit;
}

static void checker2(LCSMCircuit &circuit)
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

	// Check element's types.
	assertType(input, CircuitType::Pin);
	assertType(output1, CircuitType::Pin);
	assertType(output2, CircuitType::Pin);
	assertType(output3, CircuitType::Pin);
	assertType(tunnel1, CircuitType::Tunnel);
	assertType(tunnel2, CircuitType::Tunnel);
	assertType(tunnel3, CircuitType::Tunnel);
	assertType(tunnel4, CircuitType::Tunnel);
	assertType(wire1, CircuitType::Wire);
	assertType(wire2, CircuitType::Wire);
	assertType(wire3, CircuitType::Wire);
	assertType(wire4, CircuitType::Wire);
}

static void checker1(LCSMCircuit &circuit)
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

static void test0_pretest()
{
	// Generators.
	const GeneratorTy g1 = generator1;
	const GeneratorTy g2 = generator2;

	// Checkers.
	const CheckerTy c1 = checker1;
	const CheckerTy c2 = checker2;

	// Pre-test.
	preTest(g1, c1);
	preTest(g2, c2);
}

static void test1()
{
	LCSMCircuit circuit = generator1();

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

static void test2()
{
	LCSMCircuit circuit = generator2();

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

static void test3()
{
	LCSMCircuit circuit = generator2();

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
	model::Tunnel *tunnel2Model = static_cast< model::Tunnel * >(tunnel2);
	model::Tunnel *tunnel3Model = static_cast< model::Tunnel * >(tunnel3);
	model::Tunnel *tunnel4Model = static_cast< model::Tunnel * >(tunnel4);

	// Make additional connections.
	tunnel2Model->connectTunnel(tunnel3);
	tunnel2Model->connectTunnel(tunnel4);
	tunnel3Model->connectTunnel(tunnel2);
	tunnel3Model->connectTunnel(tunnel4);
	tunnel4Model->connectTunnel(tunnel2);
	tunnel4Model->connectTunnel(tunnel3);

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

static void test4()
{
	LCSMCircuit circuit = generator2();

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
	// Circuit *wire3 = circuit.find("wire3");
	Circuit *wire4 = circuit.find("wire4");

	// Remove the third tunnel.
	assertTrue(circuit.remove(tunnel3));

	// Extract models.
	model::Pin *inputModel = static_cast< model::Pin * >(input);
	model::Pin *output1Model = static_cast< model::Pin * >(output1);
	model::Pin *output2Model = static_cast< model::Pin * >(output2);
	model::Pin *output3Model = static_cast< model::Pin * >(output3);

	// Indexes.
	const Identifier inputId = input->id();
	const Identifier output1Id = output1->id();
	// const Identifier output2Id = output2->id();
	const Identifier output3Id = output3->id();
	const Identifier tunnel1Id = tunnel1->id();
	const Identifier tunnel2Id = tunnel2->id();
	// const Identifier tunnel3Id = tunnel3->id();
	const Identifier tunnel4Id = tunnel4->id();
	const Identifier wire1Id = wire1->id();
	const Identifier wire2Id = wire2->id();
	// const Identifier wire3Id = wire3->id();
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
			// const DataBits &output2Actual = state.valueOf(output2Id);
			const DataBits &output3Actual = state.valueOf(output3Id);
			const DataBits &tunnel1Actual = state.valueOf(tunnel1Id);
			const DataBits &tunnel2Actual = state.valueOf(tunnel2Id);
			// const DataBits &tunnel3Actual = state.valueOf(tunnel3Id);
			const DataBits &tunnel4Actual = state.valueOf(tunnel4Id);
			const DataBits &wire1Actual = state.valueOf(wire1Id);
			const DataBits &wire2Actual = state.valueOf(wire2Id);
			// const DataBits &wire3Actual = state.valueOf(wire3Id);
			const DataBits &wire4Actual = state.valueOf(wire4Id);
			assertEquals(inputActual, testdata);
			assertEquals(output1Actual, testdata);
			// assertEquals(output2Actual, testdata);
			assertEquals(output3Actual, testdata);
			assertEquals(tunnel1Actual, testdata);
			assertEquals(tunnel2Actual, testdata);
			// assertEquals(tunnel3Actual, testdata);
			assertEquals(tunnel4Actual, testdata);
			assertEquals(wire1Actual, testdata);
			assertEquals(wire2Actual, testdata);
			// assertEquals(wire3Actual, testdata);
			assertEquals(wire4Actual, testdata);
		}
	}
}

int main()
{
	// Run all tests.
	test0_pretest();
	test1();
	test2();
	test3();
	test4();
}
