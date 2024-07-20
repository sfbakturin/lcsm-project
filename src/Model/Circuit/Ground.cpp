#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Circuit/Ground.h>
#include <sim/Model/Width.h>

#include <utility>

sim::Ground::Ground() noexcept : sim::Constant(sim::Width::W1, sim::Ground::GROUND_VALUE) {}
sim::Ground::Ground(sim::Width width) noexcept : sim::Constant(width, sim::Ground::GROUND_VALUE) {}

sim::Ground::Ground(const sim::Ground &other) noexcept : sim::Constant(static_cast< sim::Constant >(other)) {}
sim::Ground::Ground(sim::Ground &&other) noexcept : sim::Constant(std::move(other)) {}

sim::Ground &sim::Ground::operator=(const sim::Ground &other) noexcept
{
	if (this != &other)
		sim::Ground(other).Swap(*this);
	return *this;
}
sim::Ground &sim::Ground::operator=(sim::Ground &&other) noexcept
{
	if (this != &other)
		sim::Ground(std::move(other)).Swap(*this);
	return *this;
}

void sim::Ground::Swap(sim::Ground &other) noexcept
{
	sim::Constant &left = *this;
	left.Swap(other);
}
