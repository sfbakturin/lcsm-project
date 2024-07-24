#include <sim/Component/WiringComponent.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/SimBuilder.h>
#include <sim/SimContext.h>

int main()
{
	sim::SimContext context;
	sim::SimCircuit dummy(context);
	sim::SimBuilder builder(dummy);

	sim::Pin *in0 = builder.CreatePin(true);
	sim::Pin *out0 = builder.CreatePin(false);

	builder.ConnectPin(in0, out0, 0);
}
