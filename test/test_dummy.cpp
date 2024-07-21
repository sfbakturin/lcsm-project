#include <sim/Component/WiringComponent.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/SimBuilder.h>
#include <sim/SimContext.h>

int main()
{
	sim::SimContext ctx;
	sim::SimBuilder builder(ctx);

	sim::Pin *input = builder.CreatePin(true);
	sim::Pin *output = builder.CreatePin(false);

	builder.ConnectPin(input, output, 0);
}
