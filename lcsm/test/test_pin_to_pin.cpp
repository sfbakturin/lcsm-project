#include <lcsm/LCSMBuilder.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Circuit/Pin.h>

int main()
{
	lcsm::LCSMCircuit circuit;
	lcsm::LCSMBuilder builder(circuit);

	/* Create pins. */
	lcsm::model::Pin *in0 = builder.CreatePin(false);
	lcsm::model::Pin *in1 = builder.CreatePin(false);

	/* Connect pins. */
	builder.ConnectPin(in0, in1, 0);
}
