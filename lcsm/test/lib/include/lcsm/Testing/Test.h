#ifndef LCSM_TESTING_TEST_H
#define LCSM_TESTING_TEST_H

#include <lcsm/LCSMCircuit.h>

#include <functional>

namespace lcsm
{
	namespace testing
	{
		using GeneratorTy = std::function< LCSMCircuit() >;
		using CheckerTy = std::function< void(LCSMCircuit &) >;
		using TestTy = std::function< void(LCSMCircuit &) >;

		void PerformTest(const GeneratorTy &generator, const CheckerTy &checker, const TestTy &test);
	}	 // namespace testing
}	 // namespace lcsm

#endif /* LCSM_TESTING_TEST_H */
