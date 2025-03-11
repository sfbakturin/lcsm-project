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

	testParseMany(inputVariants, outputVariants, { 1, 2 });
	testParseMany(outputRegVariants, inputVariants, { 3, 4 });
	testParseMany(outputVariants, inoutVariants, { 3, 4 });
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
	testNoParse("module example(input a,b,c.);", "bad symbol 1");
	testNoParse("module ex@mple(input a,b,c.);", "bad symbol 2");
	testNoParse("module example<input a,b,c>;", "bad symbol 3");
	testNoParse("", "nothing");
}

static LCSMCircuit generator1()
{
	LCSMCircuit circuit("VerilogModuleCircuitTest");

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

static void test3_pretest()
{
	// Generators.
	const GeneratorTy g1 = generator1;

	// Checkers.
	const CheckerTy c1 = checker1;

	// Pre-test.
	preTest(g1, c1);
}

static void test3_runTheVerilogModule()
{
	LCSMCircuit circuit = generator1();

	// Find circuits.
	Circuit *a = circuit.find("a");
	Circuit *b = circuit.find("b");
	Circuit *out = circuit.find("out");

	// Indexes.
	const Identifier aId = a->id();
	const Identifier bId = b->id();
	const Identifier outId = out->id();

	// Generate physical engine.
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);

	// Fork new state.
	LCSMState state = engine.fork();

	// Test!
	for (Strength sa : Strengths)
	{
		// Skip HiZ.
		// TODO: Maybe, there is another bug with icarus so that it returns 36X as value.
		if (sa == Strength::HighImpedance)
		{
			continue;
		}

		for (Strength sb : Strengths)
		{
			// Skip HiZ and HiZ.
			// TODO: Maybe, there is another bug with icarus so that it returns 36X as value.
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
					std::cout << "<a = " << va << ">, <b = " << vb << "> -> <out = " << vout << ">\n";
				}
			}
		}
	}
}

int main()
{
	test1_verilogModuleDeclareParse();
	test2_badVerilogModuleDeclareParse();
	test3_pretest();
	test3_runTheVerilogModule();
}
