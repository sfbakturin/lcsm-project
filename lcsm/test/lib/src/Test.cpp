#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Testing/Test.h>

#include <functional>
#include <iostream>
#include <utility>

void lcsm::testing::preTest(const lcsm::testing::GeneratorTy &generator, const lcsm::testing::CheckerTy &checker)
{
	// Pre-test: copy part.
	{
		// Generate original via generator.
		const lcsm::LCSMCircuit original = generator();

		// Make copy.
		lcsm::LCSMCircuit circuit = original.copy();

		// Pre-test.
		checker(circuit);
	}

	// Pre-test: move part.
	{
		// Generate original via generator.
		lcsm::LCSMCircuit original = generator();

		// Make copy.
		lcsm::LCSMCircuit circuit = std::move(original);

		// Pre-test.
		checker(circuit);
	}

	// Pre-test: must successfully created engine.
	{
		// Generate original via generator.
		const lcsm::LCSMCircuit original = generator();

		// Generate physical engine.
		lcsm::LCSMEngine engine = lcsm::LCSMEngine::fromCircuit(original);

		// Fork.
		lcsm::LCSMState state = engine.fork();
	}

	// Pre-test: must successfully dump to file.
	{
		// Generate original via generator.
		const lcsm::LCSMCircuit original = generator();

		// Dump and output.
		std::cout << "Dumping '" << original.name() << "'...\n";
		std::cout << original.dumpToString();
		std::cout << "... end of dumping\n";
	}
}
