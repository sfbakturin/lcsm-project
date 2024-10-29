
#include <lcsm/LCSMBuilder.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMContext.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/Model/Circuit/Ground.h>
#include <lcsm/Model/Circuit/Pin.h>
#include <lcsm/Model/Circuit/Power.h>
#include <lcsm/Model/Circuit/Transistor.h>
#include <lcsm/Model/Wiring/Wire.h>

int main()
{
	lcsm::LCSMContext context;
	lcsm::LCSMCircuit logicalNot(context);
	lcsm::LCSMBuilder builder(logicalNot);
	lcsm::LCSMEngine engine;

	lcsm::model::Pin *in0 = builder.CreatePin(false);
	lcsm::model::Pin *out0 = builder.CreatePin(true);
	lcsm::model::Power *pow0 = builder.CreatePower();
	lcsm::model::Ground *ground0 = builder.CreateGround();
	lcsm::model::Transistor *transistorP0 = builder.CreateTransistor(lcsm::model::TransistorType::TRANSISTOR_TYPE_P);
	lcsm::model::Transistor *transistorN0 = builder.CreateTransistor(lcsm::model::TransistorType::TRANSISTOR_TYPE_N);

	// Connect bases.
	builder.ConnectPin(in0, transistorP0, lcsm::model::Transistor::CompositeIndex::BASE);
	builder.ConnectPin(in0, transistorN0, lcsm::model::Transistor::CompositeIndex::BASE);

	// Connect power and ground.
	builder.ConnectGround(ground0, transistorN0, lcsm::model::Transistor::CompositeIndex::INOUT_A);
	builder.ConnectPower(pow0, transistorP0, lcsm::model::Transistor::CompositeIndex::INOUT_B);

	// Connect output.
	lcsm::model::Wire *wire0 =
		builder.Connect(transistorN0, lcsm::model::Transistor::CompositeIndex::INOUT_B, transistorP0, lcsm::model::Transistor::CompositeIndex::INOUT_A);
	builder.Connect(wire0, 0, out0, 0);

	// Initialize it!
	engine.addCircuit(logicalNot);
}
