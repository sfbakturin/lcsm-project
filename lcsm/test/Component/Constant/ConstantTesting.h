#ifndef LCSM_TEST_COMPONENT_CONSTANT_CONSTANTTESTING_H
#define LCSM_TEST_COMPONENT_CONSTANT_CONSTANTTESTING_H

#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

#include <tuple>

using namespace lcsm;
using namespace lcsm::verilog;

using TestData = std::tuple< Width, value_t, DataBits >;

namespace lcsm_tests
{
	static constexpr Bit T = Bit::True;
	static constexpr Bit F = Bit::False;

	LCSMCircuit generator();
	void checker(LCSMCircuit &circuit);
}	 // namespace lcsm_tests

#endif /* LCSM_TEST_COMPONENT_CONSTANT_CONSTANTTESTING_H */
