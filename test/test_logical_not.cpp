
#include <sim/Model/Circuit/Ground.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/Model/Circuit/Power.h>
#include <sim/Model/Circuit/Transistor.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/SimBuilder.h>
#include <sim/SimCircuit.h>
#include <sim/SimContext.h>
#include <sim/SimEngine.h>

int main()
{
	sim::SimContext context;
	sim::SimCircuit logicalNot(context);
	sim::SimBuilder builder(logicalNot);
	sim::SimEngine engine;

	sim::Pin *in0 = builder.CreatePin(false);
	sim::Pin *out0 = builder.CreatePin(true);
	sim::Power *pow0 = builder.CreatePower();
	sim::Ground *ground0 = builder.CreateGround();
	sim::Transistor *transistorP0 =
		builder.CreateTransistor(sim::TransistorType::TRANSISTOR_TYPE_P);
	sim::Transistor *transistorN0 =
		builder.CreateTransistor(sim::TransistorType::TRANSISTOR_TYPE_N);

	// Connect bases.
	builder.ConnectPin(in0, transistorP0, sim::Transistor::CompositeIndex::BASE);
	builder.ConnectPin(in0, transistorN0, sim::Transistor::CompositeIndex::BASE);

	// Connect power and ground.
	builder.ConnectGround(ground0, transistorN0, sim::Transistor::CompositeIndex::INOUT_A);
	builder.ConnectPower(pow0, transistorP0, sim::Transistor::CompositeIndex::INOUT_B);

	// Connect output.
	sim::Wire *wire0 = builder.Connect(
		transistorN0,
		sim::Transistor::CompositeIndex::INOUT_B,
		transistorP0,
		sim::Transistor::CompositeIndex::INOUT_A);
	builder.Connect(wire0, 0, out0, 0);

	// Initialize it!
	engine.addCircuit(logicalNot);
}
