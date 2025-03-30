#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Testing/Test.h>

#include <functional>
#include <iostream>
#include <string>
#include <utility>

void lcsm::testing::PerformTest(const lcsm::testing::GeneratorTy &generator, const lcsm::testing::CheckerTy &checker, const lcsm::testing::TestTy &test)
{
	// Pre-test: must successfully dump to file.
	std::string dump;
	{
		// Generate original via generator.
		const lcsm::LCSMCircuit original = generator();

		// Dump and output.
		dump = original.dumpToString();
		std::cout << "Dumping '" << original.name() << "'...\n";
		std::cout << dump << '\n';
		std::cout << "... end of dumping\n";
	}

	// Test: as original circuit.
	std::cout << "-- TESTING AS ORIGINAL CIRCUIT --\n";
	{
		// Generate original via generator.
		lcsm::LCSMCircuit original = generator();

		// Pre-test.
		checker(original);

		// Test.
		test(original);
	}
	std::cout << "-- END OF TESTING --\n";

	// Test: as copied circuit.
	std::cout << "-- TESTING AS COPIED CIRCUIT --\n";
	{
		// Generate original via generator.
		lcsm::LCSMCircuit original = generator();

		// Make copy.
		lcsm::LCSMCircuit circuit = original;

		// Pre-test.
		checker(circuit);

		// Test.
		test(circuit);
	}
	std::cout << "-- END OF TESTING --\n";

	// Test: as moved circuit.
	std::cout << "-- TESTING AS MOVED CIRCUIT --\n";
	{
		// Generate original via generator.
		lcsm::LCSMCircuit original = generator();

		// Make move.
		lcsm::LCSMCircuit circuit = std::move(original);

		// Pre-test.
		checker(circuit);

		// Test.
		test(circuit);
	}
	std::cout << "-- END OF TESTING --\n";

	// Test: as constructed from dump.
	std::cout << "-- TESTING AS CONSTRUCTED FROM DUMP --\n";
	{
		// Generate circuit from dump.
		lcsm::LCSMCircuit original = lcsm::LCSMCircuit::fromString(dump);

		// Pre-test.
		checker(original);

		// Test.
		test(original);
	}
	std::cout << "-- END OF TESTING --\n";
}
