#ifndef LCSM_TEST_COMPONENT_DIGIT_DIGITTESTING_H
#define LCSM_TEST_COMPONENT_DIGIT_DIGITTESTING_H

#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

using namespace lcsm;
using namespace lcsm::verilog;

namespace lcsm_tests
{
	static constexpr Bit T = Bit::True;
	static constexpr Bit F = Bit::False;

	LCSMCircuit generator();
	void checker(LCSMCircuit &circuit);
}	 // namespace lcsm_tests

#endif /* LCSM_TEST_COMPONENT_DIGIT_DIGITTESTING_H */
