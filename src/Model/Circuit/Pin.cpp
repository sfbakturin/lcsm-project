#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/Model/Width.h>

#include <utility>

sim::Pin::Pin(bool output, Width width, std::uint64_t value) : sim::Constant(width, value), m_output(output) {}

sim::Pin::Pin(const sim::Pin &other) : sim::Constant(static_cast< sim::Constant >(other)), m_output(other.m_output) {}
sim::Pin::Pin(sim::Pin &&other) noexcept : sim::Constant(std::move(other)), m_output(other.m_output) {}

sim::Pin &sim::Pin::operator=(const sim::Pin &other)
{
	if (this != &other)
		sim::Pin(other).Swap(*this);
	return *this;
}

sim::Pin &sim::Pin::operator=(sim::Pin &&other) noexcept
{
	if (this != &other)
		sim::Pin(std::move(other)).Swap(*this);
	return *this;
}

void sim::Pin::Swap(sim::Pin &other) noexcept
{
	sim::Constant &left = *this;
	left.Swap(other);

	std::swap(m_output, other.m_output);
}
