#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Splitter.h>

#include <cstddef>
#include <stdexcept>
#include <utility>

sim::Splitter::Splitter(sim::Width widthIn, std::size_t widthOut) : m_widthIn(widthIn), m_widthOut(widthOut)
{
	ResetConnectors();
}

sim::Splitter::Splitter(const sim::Splitter &other) : m_widthIn(other.m_widthIn), m_widthOut(other.m_widthOut)
{
	for (std::size_t i = 0; i < sim::Splitter::CONNECTORS_SIZE; i++)
		m_connectors[i] = other.m_connectors[i];
}
sim::Splitter::Splitter(sim::Splitter &&other) noexcept : m_widthIn(other.m_widthIn), m_widthOut(other.m_widthOut)
{
	for (std::size_t i = 0; i < sim::Splitter::CONNECTORS_SIZE; i++)
		m_connectors[i] = other.m_connectors[i];
}

sim::Splitter &sim::Splitter::operator=(const sim::Splitter &other)
{
	if (this != &other)
		sim::Splitter(other).Swap(*this);
	return *this;
}
sim::Splitter &sim::Splitter::operator=(sim::Splitter &&other) noexcept
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

void sim::Splitter::ConnectIn(const sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Splitter element have only 1 input.");
	m_wireIn.ConnectWire(wire);
}
void sim::Splitter::ConnectOut(const sim::wire_t &wire, std::size_t i)
{
	if (i >= m_widthOut)
		throw std::logic_error("Splitter element has only widthOut output connections.");
	m_wireOut[i].ConnectWire(wire);
}

void sim::Splitter::ConnectBits(const sim::wire_t &wire)
{
	ConnectIn(wire, 0);
}
void sim::Splitter::ConnectBitN(const sim::wire_t &wire, std::size_t i)
{
	ConnectOut(wire, i);
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
	const std::size_t base = m_widthIn / dem;
	const std::size_t remain = m_widthIn % dem;

	for (std::size_t i = 0; i < dem; i++)
	{
		m_connectors[i] = base;
		if (i < remain)
			m_connectors[i]++;
	}
}
