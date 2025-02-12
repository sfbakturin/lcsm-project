#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Clock::Clock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset) :
	m_highDuration(highDuration), m_lowDuration(lowDuration), m_phaseOffset(phaseOffset)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wire.connectConnect(circuit);
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
	return lcsm::support::CopyAssign< lcsm::model::Clock >(this, other);
}

lcsm::model::Clock &lcsm::model::Clock::operator=(lcsm::model::Clock &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::model::Clock >(this, std::move(other));
}

void lcsm::model::Clock::swap(lcsm::model::Clock &other) noexcept
{
	std::swap(m_highDuration, other.m_highDuration);
	std::swap(m_lowDuration, other.m_lowDuration);
	std::swap(m_phaseOffset, other.m_phaseOffset);
}

lcsm::Identifier lcsm::model::Clock::id() const noexcept
{
	return m_id;
}

lcsm::ObjectType lcsm::model::Clock::objectType() const noexcept
{
	return lcsm::ObjectType::RootInt;
}

lcsm::CircuitType lcsm::model::Clock::circuitType() const noexcept
{
	return lcsm::CircuitType::Clock;
}

lcsm::Identifier lcsm::model::Clock::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wire.identify(m_id.next());
}

void lcsm::model::Clock::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Clock::Port p = static_cast< lcsm::model::Clock::Port >(portId);
	switch (p)
	{
	case lcsm::model::Clock::Port::Wiring:
	{
		m_wire.connectToWire(circuit);
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Clock::connect(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::portid_t portId = static_cast< lcsm::portid_t >(lcsm::model::Clock::Port::Wiring);
	connect(portId, circuit);
}

lcsm::Circuit *lcsm::model::Clock::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Clock::Port p = static_cast< lcsm::model::Clock::Port >(portId);
	switch (p)
	{
	case lcsm::model::Clock::Port::Wiring:
		return std::addressof(m_wire);
	}
	return nullptr;
}
