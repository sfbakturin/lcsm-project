#ifndef LCSM_TEST_COMPONENT_CLOCK_CLOCKTESTING_H
#define LCSM_TEST_COMPONENT_CLOCK_CLOCKTESTING_H

#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Verilog/Bit.h>

#include <utility>
#include <vector>

using namespace lcsm;
using namespace lcsm::verilog;

using TestData = std::pair< DataBits, DataBits >;

namespace lcsm_tests
{
	static constexpr Bit T = Bit::True;
	static constexpr Bit F = Bit::False;

	LCSMCircuit generator();
	void checker(LCSMCircuit &circuit);
	void singleTest(LCSMState &state, const Circuit *clock, const Circuit *pin, const std::vector< TestData > &testdatas);
}	 // namespace lcsm_tests

#endif /* LCSM_TEST_COMPONENT_CLOCK_CLOCKTESTING_H */
