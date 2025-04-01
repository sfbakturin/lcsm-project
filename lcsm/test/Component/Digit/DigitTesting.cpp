#include "DigitTesting.h"

#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
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
	Component *input = circuit.find("input");
	Component *output = circuit.find("output");
	Component *digit = circuit.find("digit");
	Component *wire1 = circuit.find("wire1");
	Component *wire2 = circuit.find("wire2");

	// Check element's types.
	assertType(input, ComponentType::Pin);
	assertType(output, ComponentType::Pin);
	assertType(digit, ComponentType::Digit);
	assertType(wire1, ComponentType::Wire);
	assertType(wire2, ComponentType::Wire);
}
