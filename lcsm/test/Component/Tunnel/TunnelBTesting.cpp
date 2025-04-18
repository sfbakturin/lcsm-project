#include "TunnelBTesting.h"

#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>
#include <lcsm/Testing/Test.h>
#include <lcsm/Verilog/Bit.h>

using namespace lcsm;
using namespace lcsm::testing;

LCSMCircuit lcsm_tests::generator()
{
	LCSMCircuit circuit("TunnelTest");

	// Create all needed circuit elements.
	model::Pin *input = circuit.createPin(false, "input");
	model::Pin *output1 = circuit.createPin(true, "output1");
	model::Pin *output2 = circuit.createPin(true, "output2");
	model::Pin *output3 = circuit.createPin(true, "output3");
	model::Tunnel *tunnel1 = circuit.createTunnel("tunnel1");
	model::Tunnel *tunnel2 = circuit.createTunnel("tunnel2");
	model::Tunnel *tunnel3 = circuit.createTunnel("tunnel3");
	model::Tunnel *tunnel4 = circuit.createTunnel("tunnel4");

	// Make connections.
	tunnel1->connectTunnel(tunnel2);
	tunnel1->connectTunnel(tunnel3);
	tunnel1->connectTunnel(tunnel4);

	tunnel2->connectTunnel(tunnel1);
	tunnel3->connectTunnel(tunnel1);
	tunnel4->connectTunnel(tunnel1);

	model::Wire *wire1 = circuit.connect(input, tunnel1);
	model::Wire *wire2 = circuit.connect(tunnel2, output1);
	model::Wire *wire3 = circuit.connect(tunnel3, output2);
	model::Wire *wire4 = circuit.connect(tunnel4, output3);
	wire1->setName("wire1");
	wire2->setName("wire2");
	wire3->setName("wire3");
	wire4->setName("wire4");

	return circuit;
}

void lcsm_tests::checker(LCSMCircuit &circuit)
{
	// Find circuits.
	Component *input = circuit.find("input");
	Component *output1 = circuit.find("output1");
	Component *output2 = circuit.find("output2");
	Component *output3 = circuit.find("output3");
	Component *tunnel1 = circuit.find("tunnel1");
	Component *tunnel2 = circuit.find("tunnel2");
	Component *tunnel3 = circuit.find("tunnel3");
	Component *tunnel4 = circuit.find("tunnel4");
	Component *wire1 = circuit.find("wire1");
	Component *wire2 = circuit.find("wire2");
	Component *wire3 = circuit.find("wire3");
	Component *wire4 = circuit.find("wire4");

	// Check element's types.
	assertType(input, ComponentType::Pin);
	assertType(output1, ComponentType::Pin);
	assertType(output2, ComponentType::Pin);
	assertType(output3, ComponentType::Pin);
	assertType(tunnel1, ComponentType::Tunnel);
	assertType(tunnel2, ComponentType::Tunnel);
	assertType(tunnel3, ComponentType::Tunnel);
	assertType(tunnel4, ComponentType::Tunnel);
	assertType(wire1, ComponentType::Wire);
	assertType(wire2, ComponentType::Wire);
	assertType(wire3, ComponentType::Wire);
	assertType(wire4, ComponentType::Wire);
}
