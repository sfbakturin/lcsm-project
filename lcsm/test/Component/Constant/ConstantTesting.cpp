#include "ConstantTesting.h"

#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Testing/Asserts.h>

using namespace lcsm;
using namespace lcsm::testing;

LCSMCircuit lcsm_tests::generator()
{
	LCSMCircuit circuit("ConstantTest");

	// Create all needed circuit elements.
	model::Constant *constant = circuit.createConstant("constant");
	model::Pin *pin = circuit.createPin(true, "pin");

	// Make connections.
	model::Wire *wire = circuit.connect(constant, pin);
	wire->setName("wire");

	return circuit;
}

void lcsm_tests::checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *constant = circuit.find("constant");
	Circuit *pin = circuit.find("pin");
	Circuit *wire = circuit.find("wire");

	// Check element's types.
	assertType(constant, CircuitType::Constant);
	assertType(pin, CircuitType::Pin);
	assertType(wire, CircuitType::Wire);
}
