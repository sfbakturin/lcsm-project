#include <sim/Model/Width.h>
#include <sim/Model/Circuit/Splitter.h>
#include <cstddef>
#include <utility>

sim::Splitter::Splitter() noexcept : m_widthIn(Width::W2), m_widthOut(2)
{
	for (std::size_t i = 0; i < m_widthOut; i++)
		m_connectors[i] = i;
}
sim::Splitter::Splitter(sim::Width widthIn) noexcept : m_widthIn(widthIn), m_widthOut(2)
{
	ResetConnectors();
}
sim::Splitter::Splitter(sim::Width widthIn, std::size_t widthOut) noexcept : m_widthIn(widthIn), m_widthOut(widthOut)
{
	ResetConnectors();
}

sim::Splitter::Splitter(const sim::Splitter &other) noexcept : m_widthIn(other.m_widthIn), m_widthOut(other.m_widthOut)
{
	for (std::size_t i = 0; i < sim::Splitter::CONNECTORS_SIZE; i++)
		m_connectors[i] = other.m_connectors[i];
}
sim::Splitter::Splitter(sim::Splitter &&other) noexcept : m_widthIn(other.m_widthIn), m_widthOut(other.m_widthOut)
{
	for (std::size_t i = 0; i < sim::Splitter::CONNECTORS_SIZE; i++)
		m_connectors[i] = other.m_connectors[i];
}

sim::Splitter &sim::Splitter::operator=(const Splitter &other) noexcept
{
	if (this != &other)
		sim::Splitter(other).Swap(*this);
	return *this;
}
sim::Splitter &sim::Splitter::operator=(Splitter &&other) noexcept
{
	if (this != &other)
		sim::Splitter(std::move(other)).Swap(*this);
	return *this;
}

void sim::Splitter::Swap(sim::Splitter &other) noexcept
{
	std::swap(m_widthIn, other.m_widthIn);
	std::swap(m_widthOut, other.m_widthOut);
	for (std::size_t i = 0; i < sim::Splitter::CONNECTORS_SIZE; i++)
		std::swap(m_connectors[i], other.m_connectors[i]);
}

sim::Width sim::Splitter::GetWidthIn() const noexcept
{
	return m_widthIn;
}
std::size_t sim::Splitter::GetWidthOut() const noexcept
{
	return m_widthOut;
}

void sim::Splitter::SetWidthIn(sim::Width newWidthIn) noexcept
{
	m_widthIn = newWidthIn;
	ResetConnectors();
}
void sim::Splitter::SetWidthOut(std::size_t newWidthOut) noexcept
{
	m_widthOut = newWidthOut;
	ResetConnectors();
}

static constexpr std::size_t Min(std::size_t left, std::size_t right) noexcept
{
	return left < right ? left : right;
}

void sim::Splitter::ResetConnectors() noexcept
{
	for (std::size_t i = 0; sim::Splitter::CONNECTORS_SIZE; i++)
		m_connectors[i] = sim::Splitter::NO_CONNECT;

	const std::size_t dem = Min(m_widthIn, m_widthOut);
	const std::size_t n = m_widthIn / dem;
	const std::size_t remain = n - (n * dem);

	for (std::size_t i = 0; i < n; i++)
	{
		/* TODO */
	}
}
