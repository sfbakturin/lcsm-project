#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Pin.h>

#include <iostream>
#include <string>

using namespace lcsm;

int main()
{
	LCSMCircuit circuit("LCSMFileTest");
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output = circuit.createPin(true, "output");
	model::Pin *output2 = circuit.createPin(true, "output2");
	model::Wire *wire = circuit.connect(input, output);
	model::Wire *wire2 = circuit.connect(output2, wire);
	wire->setName("wire");
	wire2->setName("wire2");
	const std::string dump = circuit.dumpToString();
	std::cout << dump << '\n';
}
