#ifndef LCSM_TEST_COMPONENT_SPLITTER_SPLITTERTESTING_H
#define LCSM_TEST_COMPONENT_SPLITTER_SPLITTERTESTING_H

#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Splitter.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Value.h>

#include <tuple>
#include <vector>

using namespace lcsm;
using namespace lcsm::verilog;

static constexpr Bit T = Bit::True;
static constexpr Bit F = Bit::False;

using TestData2bit = std::tuple< DataBits, DataBits, DataBits >;
using TestData3bit = std::tuple< DataBits, DataBits, DataBits, DataBits >;

namespace lcsm_tests
{
	LCSMCircuit generator();
	void checker(LCSMCircuit &circuit);
	void test2bit(LCSMState &state, const Circuit *input, const Circuit *output1, const Circuit *output2, const std::vector< TestData2bit > &testdatas);
	void test3bit(LCSMState &state, const Circuit *input, const Circuit *output1, const Circuit *output2, const Circuit *output3, const std::vector< TestData3bit > &testdatas);
}	 // namespace lcsm_tests

#endif /* LCSM_TEST_COMPONENT_SPLITTER_SPLITTERTESTING_H */
