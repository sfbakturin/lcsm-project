#include <initializer_list>
#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Verilog.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/Port.h>
#include <lcsm/Verilog/Strength.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace lcsm;
using namespace lcsm::verilog;
using namespace lcsm::testing;

// Example port names.
static constexpr std::array< const char *, 4 > names = { "a", "bb", "ccc", "dddd" };

static void testGeneralParse(const char *s)
{
	std::cout << "Testing '" << s << "'...\n";

	{
		// As C string.
		const char *string = s;
		Module::fromString(string);
	}

	{
		// As c++ string.
		std::string string = s;
		Module::fromString(string);
	}
}

static void testParse(const char *s)
{
	try
	{
		testGeneralParse(s);
	} catch (const std::exception &e)
	{
		std::cerr << "Testing error! Expected nothrow, but there was an exception: '" << e.what() << "'\n";
		std::exit(EXIT_FAILURE);
	}
}

static void testNoParse(const char *s, const char *info)
{
	try
	{
		testGeneralParse(s);
		std::cerr << "Testing error! Expected throwing, but there was no exception.\n";
		std::exit(EXIT_FAILURE);
	} catch (const std::exception &e)
	{
		std::cout << "On " << info << " catch: '" << e.what() << "'\n";
	}
}

static void testParseMany(const std::vector< std::string > &variants, std::initializer_list< std::size_t > ns)
{
	for (std::size_t n : ns)
	{
		for (const std::string &variant : variants)
		{
			std::string builder = "module example(" + variant;
			bool needsComma = false;
			for (std::size_t i = 0; i < n; i++)
			{
				if (needsComma)
				{
					builder.push_back(',');
				}
				builder.push_back(' ');
				builder.append(names[i]);
				needsComma = true;
			}
			builder.append(");");
			const char *s = builder.c_str();
			testParse(s);
		}
	}
}

static void
	testParseMany(const std::vector< std::string > &variants1, const std::vector< std::string > &variants2, std::initializer_list< std::size_t > ns)
{
	for (std::size_t n : ns)
	{
		for (const std::string &variant1 : variants1)
		{
			for (const std::string &variant2 : variants2)
			{
				std::string builder = "module example(";
				builder.append(variant1);
				bool needsComma = false;
				for (std::size_t i = 0; i < n; i++)
				{
					if (needsComma)
					{
						builder.push_back(',');
					}
					builder.push_back(' ');
					builder.append(names[i]);
					needsComma = true;
				}
				builder.append(", ");
				builder.append(variant2);
				needsComma = false;
				for (std::size_t i = 0; i < n; i++)
				{
					if (needsComma)
					{
						builder.push_back(',');
					}
					builder.push_back(' ');
					builder.append(names[i]);
					needsComma = true;
				}
				builder.append(");");
				const char *s = builder.c_str();
				testParse(s);
			}
		}
	}
}

static void test1_verilogModuleDeclareParse()
{
	// No arguments.
	testParse("module example();");

	// All 'output' variants.
	// clang-format off
	const std::vector< std::string > outputVariants = {
		"output",
		"output wire",
		"output wire [0:0]",
		"output wire [12:0]",
		"output wire [0:12]",
		"output wire [13:12]",
		"output [0:0]",
		"output [12:0]",
		"output [0:12]",
		"output [13:12]",
	};
	// clang-format on

	testParseMany(outputVariants, { 1 });
	testParseMany(outputVariants, { 2, 3 });

	// All 'output reg' variants.
	// clang-format off
	const std::vector< std::string > outputRegVariants = {
		"output reg",
		"output reg [0:0]",
		"output reg [12:0]",
		"output reg [0:12]",
		"output reg [13:12]",
	};
	// clang-format on

	testParseMany(outputRegVariants, { 1 });
	testParseMany(outputRegVariants, { 2, 3 });

	// All 'input' variants.
	// clang-format off
	const std::vector< std::string > inputVariants = {
		"input",
		"input wire",
		"input wire [0:0]",
		"input wire [12:0]",
		"input wire [0:12]",
		"input wire [13:12]",
		"input [0:0]",
		"input [12:0]",
		"input [0:12]",
		"input [13:12]",
	};
	// clang-format on

	testParseMany(inputVariants, { 1 });
	testParseMany(inputVariants, { 2, 3 });

	// All 'input reg' variants.
	// clang-format off
	const std::vector< std::string > inputRegVariants = {
		"input reg",
		"input reg [0:0]",
		"input reg [12:0]",
		"input reg [0:12]",
		"input reg [13:12]",
	};
	// clang-format on

	testParseMany(inputRegVariants, { 1 });
	testParseMany(inputRegVariants, { 2, 3 });

	// All 'inout' variants.
	// clang-format off
	const std::vector< std::string > inoutVariants = {
		"inout",
		"inout wire",
		"inout wire [0:0]",
		"inout wire [12:0]",
		"inout wire [0:12]",
		"inout wire [13:12]",
		"inout [0:0]",
		"inout [12:0]",
		"inout [0:12]",
		"inout [13:12]",
	};
	// clang-format on

	testParseMany(inoutVariants, { 1 });
	testParseMany(inoutVariants, { 2, 3 });

	// All 'inout reg' variants.
	// clang-format off
	const std::vector< std::string > inoutRegVariants = {
		"inout reg",
		"inout reg [0:0]",
		"inout reg [12:0]",
		"inout reg [0:12]",
		"inout reg [13:12]",
	};
	// clang-format on

	testParseMany(inoutRegVariants, { 1 });
	testParseMany(inoutRegVariants, { 2, 3 });

	testParseMany(inputVariants, outputVariants, { 1, 2 });
	testParseMany(outputRegVariants, inputVariants, { 3, 4 });
	testParseMany(outputVariants, inoutVariants, { 3, 4 });
	testParseMany(inputRegVariants, outputRegVariants, { 1, 2 });
	testParseMany(outputRegVariants, inputRegVariants, { 3, 4 });
	testParseMany(outputRegVariants, inoutRegVariants, { 3, 4 });
}

static void test2_badVerilogModuleDeclareParse()
{
	testNoParse("m", "bad 'module' keyword 1");
	testNoParse("mod", "bad 'module' keyword 2");
	testNoParse("modul", "bad 'module' keyword 3");
	testNoParse("module ();", "no module identifier 1");
	testNoParse("module (input a);", "no module identifier 2");
	testNoParse("module example()", "no semicolon 1");
	testNoParse("module example(input a)", "no semicolon 2");
	testNoParse("module example example();", "identifier continues with another identifier 1");
	testNoParse("module example example(input a);", "identifier continues with another identifier 2");
	testNoParse("module example(input a,);", "no list of identifier 1");
	testNoParse("module example(input a,b,c,);", "no list of identifier 2");
	testNoParse("module example(input output a,b,c);", "multiply port directions 1");
	testNoParse("module example(output inout a,b,c);", "multiply port directions 2");
	testNoParse("module example(input a,b,c.);", "bad symbol 1");
	testNoParse("module ex@mple(input a,b,c.);", "bad symbol 2");
	testNoParse("module example<input a,b,c>;", "bad symbol 3");
	testNoParse("", "nothing");
}

static LCSMCircuit generator1()
{
	LCSMCircuit circuit("VerilogModuleCircuitTest1");

	// Create all needed circuit elements.
	model::Pin *a = circuit.createPin(false, "a");
	model::Pin *b = circuit.createPin(false, "b");
	model::Pin *out = circuit.createPin(true, "out");

	// Create Verilog Module.
	Module module = Module::fromString(
		"module strength_example1(output wire [0:0] out, input a, b);\nor(strong1, "
		"weak0) o1(out, a, b);\nand(weak1, weak0) o2(out, a, b);\nendmodule");
	model::VerilogModule *strength_example1 = circuit.createVerilogModule(std::move(module));

	// Make connections.
	circuit.connect(a, strength_example1, strength_example1->indexOfInput(0));
	circuit.connect(b, strength_example1, strength_example1->indexOfInput(1));
	circuit.connect(out, strength_example1, strength_example1->indexOfOutput(0));

	return circuit;
}

static LCSMCircuit generator2()
{
	LCSMCircuit circuit("VerilogModuleCircuitTest2");

	// Create all needed circuit elements.
	model::Pin *in = circuit.createPin(false, "in", Width::Bit2);
	model::Pin *out = circuit.createPin(true, "out");

	// Create Verilog Module.
	Module module = Module::fromString(
		"module or_test1(output wire out, input [1:0] in);or (pull1, strong0) o1(out, "
		"in[0], in[1]);endmodule");
	model::VerilogModule *or_test1 = circuit.createVerilogModule(std::move(module));

	// Make connections.
	circuit.connect(in, or_test1, or_test1->indexOfInputByLabel("in"));
	circuit.connect(or_test1, or_test1->indexOfOutputByLabel("out"), out);

	return circuit;
}

static LCSMCircuit generator3()
{
	LCSMCircuit circuit("VerilogModuleCircuitTest3");

	// Create all needed circuit elements.
	model::Pin *in = circuit.createPin(false, "in", Width::Bit2);
	model::Pin *out = circuit.createPin(true, "out");

	// Create Verilog Module.
	Module module = Module::fromString(
		"module or_test2(output wire out, input [1:2] in);or (weak1, pull0) o1(out, "
		"in[2], in[1]);endmodule");
	model::VerilogModule *or_test2 = circuit.createVerilogModule(std::move(module));

	// Make connections.
	circuit.connect(in, or_test2, or_test2->indexOfInputByLabel("in"));
	circuit.connect(or_test2, or_test2->indexOfOutputByLabel("out"), out);

	return circuit;
}

static LCSMCircuit generator4()
{
	LCSMCircuit circuit("VerilogModuleCircuitTest4");

	// Create all needed circuit elements.
	model::Pin *in = circuit.createPin(false, "in", Width::Bit2);
	model::Pin *out = circuit.createPin(true, "out", Width::Bit2);

	// Create Verilog Module.
	Module module = Module::fromString(R"(
		module andor_test(input [1:0] in, output [0:1] out);
			and (strong0, strong1) o1(out[0], in[0], in[1]);
			or (strong0, strong1) o2(out[1], in[0], in[1]);
		endmodule
	)");
	model::VerilogModule *andor_test = circuit.createVerilogModule(std::move(module));

	// Make connections.
	circuit.connect(in, andor_test, andor_test->indexOfByLabel("in"));
	circuit.connect(andor_test, andor_test->indexOfByLabel("out"), out);

	return circuit;
}

static LCSMCircuit generator5()
{
	LCSMCircuit circuit("VerilogModuleCircuitTest5");

	// Create all needed circuit elements.
	model::Pin *in = circuit.createPin(false, "in");
	model::Pin *out = circuit.createPin(true, "out");

	// Create Verilog Module.
	Module module = Module::fromString(R"(
		module reg_test(output reg out, input reg in);
			not(out, in);
		endmodule
	)");
	model::VerilogModule *reg_test = circuit.createVerilogModule(std::move(module));

	// Make connections.
	circuit.connect(in, reg_test, reg_test->indexOfByLabel("in"));
	circuit.connect(reg_test, reg_test->indexOfByLabel("out"), out);

	return circuit;
}

static void checker1(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *a = circuit.find("a");
	Circuit *b = circuit.find("b");
	Circuit *out = circuit.find("out");
	Circuit *strength_example1 = circuit.find("strength_example1");

	// Check element's types.
	assertType(a, CircuitType::Pin);
	assertType(b, CircuitType::Pin);
	assertType(out, CircuitType::Pin);
	assertType(strength_example1, CircuitType::VerilogModule);
}

static void checker2(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *in = circuit.find("in");
	Circuit *out = circuit.find("out");
	Circuit *or_test1 = circuit.find("or_test1");

	// Check element's types.
	assertType(in, CircuitType::Pin);
	assertType(out, CircuitType::Pin);
	assertType(or_test1, CircuitType::VerilogModule);
}

static void checker3(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *in = circuit.find("in");
	Circuit *out = circuit.find("out");
	Circuit *or_test2 = circuit.find("or_test2");

	// Check element's types.
	assertType(in, CircuitType::Pin);
	assertType(out, CircuitType::Pin);
	assertType(or_test2, CircuitType::VerilogModule);
}

static void checker4(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *in = circuit.find("in");
	Circuit *out = circuit.find("out");
	Circuit *andor_test = circuit.find("andor_test");

	// Check element's types.
	assertType(in, CircuitType::Pin);
	assertType(out, CircuitType::Pin);
	assertType(andor_test, CircuitType::VerilogModule);
}

static void checker5(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *in = circuit.find("in");
	Circuit *out = circuit.find("out");
	Circuit *reg_test = circuit.find("reg_test");

	// Check element's types.
	assertType(in, CircuitType::Pin);
	assertType(out, CircuitType::Pin);
	assertType(reg_test, CircuitType::VerilogModule);
}

static void test3_runTheVerilogModule(LCSMCircuit &circuit)
{
	// LCSMCircuit circuit = generator1();

	// Find circuits.
	Circuit *a = circuit.find("a");
	Circuit *b = circuit.find("b");
	Circuit *out = circuit.find("out");
	Circuit *strengthExample = circuit.find("strength_example1");

	// Extract models.
	model::VerilogModule *strengthExampleModel = static_cast< model::VerilogModule * >(strengthExample);

	// Indexes.
	const Identifier aId = a->id();
	const Identifier bId = b->id();
	const Identifier outId = out->id();
	const Identifier strengthExampleId = strengthExample->id();

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (Strength sa : Strengths)
	{
		// Skip HiZ.
		// No ambiguous strength.
		if (sa == Strength::HighImpedance)
		{
			continue;
		}

		for (Strength sb : Strengths)
		{
			// Skip HiZ and HiZ.
			// No ambiguous strength.
			if (sb == Strength::HighImpedance)
			{
				continue;
			}

			for (Bit ba : Bits)
			{
				for (Bit bb : Bits)
				{
					// Inputs.
					const DataBits va{ Width::Bit1, sa, ba };
					const DataBits vb{ Width::Bit1, sb, bb };

					// Put values.
					state.putValue(aId, va);
					state.putValue(bId, vb);

					// Step once.
					state.tick();

					// Printout values.
					const DataBits &vout = state.valueOf(outId);
					const DataBits &mout = state.valueOf(strengthExampleId, strengthExampleModel->indexOfByLabel("out"));
					const DataBits &ma = state.valueOf(strengthExampleId, strengthExampleModel->indexOfByLabel("a"));
					const DataBits &mb = state.valueOf(strengthExampleId, strengthExampleModel->indexOfByLabel("b"));
					assertEquals(vout, mout);
					assertEquals(ma, va);
					assertEquals(mb, vb);
					std::cout << "<a = " << va << ">, <b = " << vb << "> -> <out = " << vout << ">\n";
				}
			}
		}
	}
}

static void test4_runTheVerilogModuleWithWideWires1(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *in = circuit.find("in");
	Circuit *out = circuit.find("out");
	Circuit *orTest = circuit.find("or_test1");

	// Extract models.
	model::VerilogModule *orTestModel = static_cast< model::VerilogModule * >(orTest);

	// Indexes.
	const Identifier inId = in->id();
	const Identifier outId = out->id();
	const Identifier orTestId = orTest->id();

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (Strength s1 : Strengths)
	{
		// Skip HiZ and HiZ.
		// No ambiguous strength.
		if (s1 == Strength::HighImpedance)
		{
			continue;
		}

		for (Strength s2 : Strengths)
		{
			// Skip HiZ and HiZ.
			// No ambiguous strength.
			if (s2 == Strength::HighImpedance)
			{
				continue;
			}

			for (Bit bit1 : Bits)
			{
				for (Bit bit2 : Bits)
				{
					// Input.
					std::initializer_list< Value > vins = { { s1, bit1 }, { s2, bit2 } };
					const DataBits vin{ vins };

					// Put value.
					state.putValue(inId, vin);

					// Step once.
					state.tick();

					// Assertion.
					const DataBits &vout = state.valueOf(outId);
					const DataBits &mdlin = state.valueOf(orTestId, orTestModel->indexOfByLabel("in"));
					const DataBits &mdlout = state.valueOf(orTestId, orTestModel->indexOfByLabel("out"));
					assertEquals(vout, mdlout);
					assertEquals(vin, mdlin);
					std::cout << "<in = " << vin << ">, <out = " << vout << ">\n";
				}
			}
		}
	}
}

static void test4_runTheVerilogModuleWithWideWires2(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *in = circuit.find("in");
	Circuit *out = circuit.find("out");
	Circuit *orTest = circuit.find("or_test2");

	// Extract models.
	model::VerilogModule *orTestModel = static_cast< model::VerilogModule * >(orTest);

	// Indexes.
	const Identifier inId = in->id();
	const Identifier outId = out->id();
	const Identifier orTestId = orTest->id();

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (Strength s1 : Strengths)
	{
		// Skip HiZ and HiZ.
		// No ambiguous strength.
		if (s1 == Strength::HighImpedance)
		{
			continue;
		}

		for (Strength s2 : Strengths)
		{
			// Skip HiZ and HiZ.
			// No ambiguous strength.
			if (s2 == Strength::HighImpedance)
			{
				continue;
			}

			for (Bit bit1 : Bits)
			{
				for (Bit bit2 : Bits)
				{
					// Input.
					std::initializer_list< Value > vins = { { s1, bit1 }, { s2, bit2 } };
					const DataBits vin{ vins };

					// Put value.
					state.putValue(inId, vin);

					// Step once.
					state.tick();

					// Assertion.
					const DataBits &vout = state.valueOf(outId);
					const DataBits &mdlin = state.valueOf(orTestId, orTestModel->indexOfByLabel("in"));
					const DataBits &mdlout = state.valueOf(orTestId, orTestModel->indexOfByLabel("out"));
					assertEquals(vout, mdlout);
					assertEquals(vin, mdlin);
					std::cout << "<in = " << vin << ">, <out = " << vout << ">\n";
				}
			}
		}
	}
}

static void test5_runTheVerilogModuleWithWideOut(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *in = circuit.find("in");
	Circuit *out = circuit.find("out");
	Circuit *andOrTest = circuit.find("andor_test");

	// Extract models.
	model::VerilogModule *orTestModel = static_cast< model::VerilogModule * >(andOrTest);

	// Indexes.
	const Identifier inId = in->id();
	const Identifier outId = out->id();
	const Identifier andOrTestId = andOrTest->id();

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (Strength s1 : Strengths)
	{
		// Skip HiZ and HiZ.
		// No ambiguous strength.
		if (s1 == Strength::HighImpedance)
		{
			continue;
		}

		for (Strength s2 : Strengths)
		{
			// Skip HiZ and HiZ.
			// No ambiguous strength.
			if (s2 == Strength::HighImpedance)
			{
				continue;
			}

			for (Bit bit1 : Bits)
			{
				for (Bit bit2 : Bits)
				{
					// Input.
					std::initializer_list< Value > vins = { { s1, bit1 }, { s2, bit2 } };
					const DataBits vin{ vins };

					// Put value.
					state.putValue(inId, vin);

					// Step once.
					state.tick();

					// Assertion.
					const DataBits &vout = state.valueOf(outId);
					const DataBits &mdlin = state.valueOf(andOrTestId, orTestModel->indexOfByLabel("in"));
					const DataBits &mdlout = state.valueOf(andOrTestId, orTestModel->indexOfByLabel("out"));
					assertEquals(vout, mdlout);
					assertEquals(vin, mdlin);
					std::cout << "<in = " << vin << ">, <out = " << vout << ">\n";
				}
			}
		}
	}
}

static void test6_runTheVerilogModuleWithRegs(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *in = circuit.find("in");
	Circuit *out = circuit.find("out");
	Circuit *regTest = circuit.find("reg_test");

	// Extract models.
	model::VerilogModule *regTestModel = static_cast< model::VerilogModule * >(regTest);

	// Indexes.
	const Identifier inId = in->id();
	const Identifier outId = out->id();
	const Identifier regTestId = regTest->id();

	// Test data.
	// clang-format off
	const std::vector< std::pair< Bit, Bit > > testdatas = {
		{ Bit::True,      Bit::False     },
		{ Bit::False,     Bit::True      },
		{ Bit::Undefined, Bit::Undefined }
	};
	// clang-format on

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (Strength s : Strengths)
	{
		// Skip HiZ.
		if (s == Strength::HighImpedance)
		{
			continue;
		}

		for (const std::pair< Bit, Bit > &testdata : testdatas)
		{
			// Testing data.
			const Bit inputBit = testdata.first;
			const Bit expectedBit = testdata.second;
			const DataBits input{ Width::Bit1, s, inputBit };
			const DataBits expected{ Width::Bit1, Strength::StrongDrive, expectedBit };

			// Put value.
			state.putValue(inId, input);

			// Step once.
			state.tick();

			// Assertion.
			const DataBits &output = state.valueOf(outId);
			const DataBits &verilogIn = state.valueOf(regTestId, regTestModel->indexOfByLabel("in"));
			const DataBits &verilogOut = state.valueOf(regTestId, regTestModel->indexOfByLabel("out"));
			assertEquals(output, expected);
			assertEquals(verilogIn, input);
			assertEquals(verilogOut, output);
			std::cout << "<in = " << input << ">, <out = " << output << ">\n";
		}
	}

	// Test with HiZ.
	for (Bit b : Bits)
	{
		// Testing data.
		const DataBits input{ Width::Bit1, Strength::HighImpedance, b };
		const DataBits expected{ Width::Bit1, Strength::StrongDrive, Bit::Undefined };

		// Put value.
		state.putValue(inId, input);

		// Step once.
		state.tick();

		// Assertion.
		const DataBits &output = state.valueOf(outId);
		const DataBits &verilogIn = state.valueOf(regTestId, regTestModel->indexOfByLabel("in"));
		const DataBits &verilogOut = state.valueOf(regTestId, regTestModel->indexOfByLabel("out"));
		assertEquals(output, expected);
		assertEquals(verilogIn, input);
		assertEquals(verilogOut, output);
		std::cout << "<in = " << input << ">, <out = " << output << ">\n";
	}
}

int main()
{
	// Run all tests.
	test1_verilogModuleDeclareParse();
	test2_badVerilogModuleDeclareParse();
	PerformTest(generator1, checker1, test3_runTheVerilogModule);
	PerformTest(generator2, checker2, test4_runTheVerilogModuleWithWideWires1);
	PerformTest(generator3, checker3, test4_runTheVerilogModuleWithWideWires2);
	PerformTest(generator4, checker4, test5_runTheVerilogModuleWithWideOut);
	PerformTest(generator5, checker5, test6_runTheVerilogModuleWithRegs);
}
