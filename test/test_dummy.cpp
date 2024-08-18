#include <sim/Component/WiringComponent.h>
#include <sim/IR/Bit.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/SimBuilder.h>
#include <sim/SimContext.h>
#include <sim/SimEngine.h>

int main()
{
	sim::SimContext context;
	sim::SimCircuit dummy(context);
	sim::SimBuilder builder(dummy);
	sim::SimEngine engine;

	sim::Pin *in0 = builder.CreatePin(true);
	sim::Pin *out0 = builder.CreatePin(false);

	builder.ConnectPin(in0, out0, 0);

	engine.addCircuit(dummy);

	engine.invokeFull({ { sim::LogisimBit::LOGISIM_TRUE } });
}
