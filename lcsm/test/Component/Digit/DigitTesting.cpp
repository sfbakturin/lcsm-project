#include "DigitTesting.h"

#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Digit.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Testing/Asserts.h>

using namespace lcsm;
using namespace lcsm::testing;

LCSMCircuit lcsm_tests::generator()
{
	LCSMCircuit circuit("DigitTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");
	model::Digit *digit = circuit.createDigit("digit");

	// Make connections.
	model::Wire *wire1 = circuit.connect(input, output);
	model::Wire *wire2 = circuit.connect(wire1, digit);
	wire1->setName("wire1");
	wire2->setName("wire2");

	return circuit;
}

void lcsm_tests::checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Circuit *input = circuit.find("input");
	Circuit *output = circuit.find("output");
	Circuit *digit = circuit.find("digit");
	Circuit *wire1 = circuit.find("wire1");
	Circuit *wire2 = circuit.find("wire2");

	// Check element's types.
	assertType(input, CircuitType::Pin);
	assertType(output, CircuitType::Pin);
	assertType(digit, CircuitType::Digit);
	assertType(wire1, CircuitType::Wire);
	assertType(wire2, CircuitType::Wire);
}
