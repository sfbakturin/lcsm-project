
#include <sim/SimBuilder.h>
#include <sim/SimCircuit.h>
#include <sim/SimContext.h>
#include <sim/SimEngine.h>

int main()
{
	sim::SimContext context;
	sim::SimCircuit dummy(context);
	sim::SimBuilder builder(dummy);
	sim::SimEngine engine;
}
