#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Circuit/Power.h>

#include <utility>

sim::Power::Power(sim::Width width) : sim::Constant(width, sim::Power::POWER_VALUE) {}

sim::Power::Power(const sim::Power &other) : sim::Constant(static_cast< sim::Constant >(other)) {}
sim::Power::Power(sim::Power &&other) noexcept : sim::Constant(std::move(other)) {}

sim::Power &sim::Power::operator=(const sim::Power &other)
{
	if (this != &other)
		sim::Power(other).Swap(*this);
	return *this;
}
sim::Power &sim::Power::operator=(sim::Power &&other) noexcept
{
	if (this != &other)
		sim::Power(std::move(other)).Swap(*this);
	return *this;
}

void sim::Power::Swap(sim::Power &other) noexcept
{
	sim::Constant &left = *this;
	left.Swap(other);
}
