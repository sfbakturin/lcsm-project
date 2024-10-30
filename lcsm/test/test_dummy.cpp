#include <lcsm/Component/WiringComponent.h>
#include <lcsm/LCSMBuilder.h>
#include <lcsm/LCSMContext.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/Model/Circuit/Pin.h>

int main()
{
	lcsm::LCSMContext context;
	lcsm::LCSMCircuit dummy(context);
	lcsm::LCSMBuilder builder(dummy);
	lcsm::LCSMEngine engine;

	lcsm::model::Pin *in0 = builder.CreatePin(false);
	lcsm::model::Pin *out0 = builder.CreatePin(true);

	builder.ConnectPin(in0, out0, 0);

	engine.addCircuit(dummy);
}
