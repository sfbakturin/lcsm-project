#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Pin.h>

#include <iostream>
#include <string>

using namespace lcsm;

int main()
{
	LCSMCircuit circuit("LCSMFileTest");

	// Create all needed components.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");
	model::Pin *output2 = circuit.createPin(true, "output2");

	// Make connections.
	model::Wire *wire = circuit.connect(input, output);
	model::Wire *wire2 = circuit.connect(output2, wire);
	wire->setName("wire");
	wire2->setName("wire2");

	// Dump.
	const std::string dump = circuit.dumpToString();
	std::cout << dump << '\n';
}
