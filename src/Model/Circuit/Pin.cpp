#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Width.h>
#include <sim/Model/Circuit/Pin.h>

#include <utility>

sim::Pin::Pin() noexcept : sim::Constant(sim::Width::W1, 0), m_output(false) {}
sim::Pin::Pin(bool output) noexcept : sim::Constant(sim::Width::W1, 0), m_output(output) {}
sim::Pin::Pin(bool output, Width width) noexcept : sim::Constant(width, 0), m_output(output) {}
sim::Pin::Pin(bool output, Width width, std::uint64_t value) noexcept : sim::Constant(width, value), m_output(output) {}

sim::Pin::Pin(const Pin &other) noexcept : sim::Constant(static_cast<sim::Constant>(other)), m_output(other.m_output) {}
sim::Pin::Pin(Pin &&other) noexcept : sim::Constant(std::move(static_cast<sim::Constant>(other))), m_output(other.m_output) {}

sim::Pin &sim::Pin::operator=(const sim::Pin &other) noexcept
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
	sim::Constant &right = other;
	left.Swap(other);

	std::swap(m_output, other.m_output);
}
