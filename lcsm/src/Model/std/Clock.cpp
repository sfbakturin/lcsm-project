#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Clock.h>
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

unsigned lcsm::model::Clock::highDuration() const noexcept
{
	return m_highDuration;
}

void lcsm::model::Clock::setHighDuration(unsigned highDuration) noexcept
{
	m_highDuration = highDuration;
}

unsigned lcsm::model::Clock::lowDuration() const noexcept
{
	return m_lowDuration;
}

void lcsm::model::Clock::setLowDuration(unsigned lowDuration) noexcept
{
	m_lowDuration = lowDuration;
}

unsigned lcsm::model::Clock::phaseOffset() const noexcept
{
	return m_phaseOffset;
}

void lcsm::model::Clock::setPhaseOffset(unsigned phaseOffset) noexcept
{
	m_phaseOffset = phaseOffset;
}

const lcsm::model::Wire &lcsm::model::Clock::wire() const noexcept
{
	return m_wire;
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
		m_wire.connectToWire(circuit);
		break;
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
