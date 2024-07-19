#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Circuit/Power.h>
#include <sim/Model/Width.h>

#include <utility>

sim::Power::Power() noexcept : sim::Constant(sim::Width::W1, sim::Power::POWER_VALUE) {}
sim::Power::Power(sim::Width width) noexcept : sim::Constant(width, sim::Power::POWER_VALUE) {}

sim::Power::Power(const Power &other) noexcept : sim::Constant(static_cast<sim::Constant>(other)) {}
sim::Power::Power(Power &&other) noexcept : sim::Constant(std::move(static_cast<sim::Constant>(other))) {}

sim::Power &sim::Power::operator=(const Power &other) noexcept
{
	if (this != &other)
		sim::Power(other).Swap(*this);
	return *this;
}
sim::Power &sim::Power::operator=(Power &&other) noexcept
{
	if (this != &other)
		sim::Power(std::move(other)).Swap(*this);
	return *this;
}

void sim::Power::Swap(sim::Power &other) noexcept
{
	sim::Constant &left = *this;
	sim::Constant &right = other;
	left.Swap(other);
}
