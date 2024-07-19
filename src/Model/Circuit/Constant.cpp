#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Width.h>

#include <utility>

sim::Constant::Constant() noexcept : m_width(sim::Width::W1), m_value(1) {}
sim::Constant::Constant(sim::Width width) noexcept : m_width(width), m_value(1) {}
sim::Constant::Constant(sim::Width width, std::uint64_t value) noexcept : m_width(width), m_value(value) {}

sim::Constant::Constant(const sim::Constant &other) noexcept : m_width(other.m_width), m_value(other.m_value) {}
sim::Constant::Constant(sim::Constant &&other) noexcept : m_width(other.m_width), m_value(other.m_value) {}

sim::Constant &sim::Constant::operator=(const sim::Constant &other) noexcept
{
	if (this != &other)
	{
		Constant(other).Swap(*this);
	}

	return *this;
}
sim::Constant &sim::Constant::operator=(sim::Constant &&other) noexcept
{
	if (this != &other)
	{
		Constant(std::move(other)).Swap(*this);
	}

	return *this;
}

void sim::Constant::Swap(sim::Constant &other) noexcept
{
	std::swap(m_value, other.m_value);
	std::swap(m_width, other.m_width);
}

sim::Width sim::Constant::GetWidth() const noexcept
{
	return m_width;
}
std::uint64_t sim::Constant::GetValue() const noexcept
{
	return m_value;
}

void sim::Constant::SetWidth(sim::Width newWidth) noexcept
{
	m_width = newWidth;
}
void sim::Constant::SetValue(std::uint64_t newValue) noexcept
{
	m_value = newValue;
}
