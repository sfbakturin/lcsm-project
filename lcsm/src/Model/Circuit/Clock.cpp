#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/Model/Circuit/Clock.h>

#include <cstddef>
#include <stdexcept>
#include <utility>

lcsm::model::Clock::Clock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset) :
	m_highDuration(highDuration), m_lowDuration(lowDuration), m_phaseOffset(phaseOffset), m_wireOut(this)
{
}

lcsm::model::Clock::Clock(const lcsm::model::Clock &other) :
	m_highDuration(other.m_highDuration), m_lowDuration(other.m_lowDuration), m_phaseOffset(other.m_phaseOffset)
{
}

lcsm::model::Clock::Clock(lcsm::model::Clock &&other) noexcept :
	m_highDuration(other.m_highDuration), m_lowDuration(other.m_lowDuration), m_phaseOffset(other.m_phaseOffset)
{
}

lcsm::model::Clock &lcsm::model::Clock::operator=(const lcsm::model::Clock &other)
{
	if (this != &other)
		lcsm::model::Clock(other).swap(*this);
	return *this;
}

lcsm::model::Clock &lcsm::model::Clock::operator=(lcsm::model::Clock &&other) noexcept
{
	if (this != &other)
		lcsm::model::Clock(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Clock::swap(lcsm::model::Clock &other) noexcept
{
	std::swap(m_highDuration, other.m_highDuration);
	std::swap(m_lowDuration, other.m_lowDuration);
	std::swap(m_phaseOffset, other.m_phaseOffset);
}

lcsm::Identifier lcsm::model::Clock::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Clock::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wireOut.identify(m_id.next());
}

void lcsm::model::Clock::connectIn(lcsm::wire_t &, std::size_t)
{
	throw std::logic_error("Clock element doesn't have inputs.");
}

void lcsm::model::Clock::connectOut(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Clock element has only 1 output.");
	m_wireOut.connectWire(wire);
	wire->connectWire(m_wireOut);
}

unsigned lcsm::model::Clock::highDuration() const noexcept
{
	return m_highDuration;
}

unsigned lcsm::model::Clock::lowDuration() const noexcept
{
	return m_lowDuration;
}

unsigned lcsm::model::Clock::phaseOffset() const noexcept
{
	return m_phaseOffset;
}

void lcsm::model::Clock::setHighDuration(unsigned newHighDuration) noexcept
{
	m_highDuration = newHighDuration;
}

void lcsm::model::Clock::setLowDuration(unsigned newLowDuration) noexcept
{
	m_lowDuration = newLowDuration;
}

void lcsm::model::Clock::setPhaseOffset(unsigned newPhaseOffset) noexcept
{
	m_phaseOffset = newPhaseOffset;
}

void lcsm::model::Clock::connect(lcsm::wire_t &wire)
{
	connectOut(wire, 0);
}

lcsm::CircuitComponentType lcsm::model::Clock::circuitComponentType() const noexcept
{
	return lcsm::CircuitComponentType::CIRCUIT_COMP_CLOCK;
}
