#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Tunnel.h>

using namespace lcsm;
using namespace lcsm::support;

static LCSMCircuit getCircuit()
{
	LCSMCircuit circuit;
	model::Pin *pin1 = circuit.createPin(false);
	model::Pin *pin2 = circuit.createPin(true);
	model::Tunnel *tunnel1 = circuit.createTunnel();
	model::Tunnel *tunnel2 = circuit.createTunnel();
	tunnel1->connectTunnel(tunnel2);
	tunnel2->connectTunnel(tunnel1);
	circuit.connect(pin1, tunnel1);
	circuit.connect(pin2, tunnel2);
	return circuit;
}

int main()
{
	LCSMCircuit circuit = getCircuit();
	LCSMEngine engine = LCSMEngine::fromCircuit(circuit);
	LCSMState state = engine.fork();
}
