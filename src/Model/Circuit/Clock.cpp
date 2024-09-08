#include <sim/Component/Identifier.h>
#include <sim/Component/WiringComponent.h>
#include <sim/Model/Circuit/Clock.h>

#include <cstddef>
#include <stdexcept>
#include <utility>

sim::Clock::Clock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset) :
	m_highDuration(highDuration), m_lowDuration(lowDuration), m_phaseOffset(phaseOffset)
{
}

sim::Clock::Clock(const sim::Clock &other) :
	m_highDuration(other.m_highDuration), m_lowDuration(other.m_lowDuration),
	m_phaseOffset(other.m_phaseOffset)
{
}

sim::Clock::Clock(sim::Clock &&other) noexcept :
	m_highDuration(other.m_highDuration), m_lowDuration(other.m_lowDuration),
	m_phaseOffset(other.m_phaseOffset)
{
}

sim::Clock &sim::Clock::operator=(const sim::Clock &other)
{
	if (this != &other)
		sim::Clock(other).Swap(*this);
	return *this;
}

sim::Clock &sim::Clock::operator=(sim::Clock &&other) noexcept
{
	if (this != &other)
		sim::Clock(std::move(other)).Swap(*this);
	return *this;
}

void sim::Clock::Swap(sim::Clock &other) noexcept
{
	std::swap(m_highDuration, other.m_highDuration);
	std::swap(m_lowDuration, other.m_lowDuration);
	std::swap(m_phaseOffset, other.m_phaseOffset);
}

sim::Identifier sim::Clock::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Clock::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wireOut.identify(m_id.next());
}

void sim::Clock::ConnectIn(sim::wire_t &, std::size_t)
{
	throw std::logic_error("Clock element doesn't have inputs.");
}

void sim::Clock::ConnectOut(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Clock element has only 1 output.");
	m_wireOut.ConnectWire(wire);
}

unsigned sim::Clock::GetHighDuration() const noexcept
{
	return m_highDuration;
}

unsigned sim::Clock::GetLowDuration() const noexcept
{
	return m_lowDuration;
}

unsigned sim::Clock::GetPhaseOffset() const noexcept
{
	return m_phaseOffset;
}

void sim::Clock::SetHighDuration(unsigned newHighDuration) noexcept
{
	m_highDuration = newHighDuration;
}

void sim::Clock::GetLowDuration(unsigned newLowDuration) noexcept
{
	m_lowDuration = newLowDuration;
}

void sim::Clock::GetPhaseOffset(unsigned newPhaseOffset) noexcept
{
	m_phaseOffset = newPhaseOffset;
}

void sim::Clock::Connect(sim::wire_t &wire)
{
	ConnectOut(wire, 0);
}

sim::CircuitComponentType sim::Clock::circuitComponentType() const noexcept
{
	return sim::CircuitComponentType::CIRCUIT_COMP_CLOCK;
}
