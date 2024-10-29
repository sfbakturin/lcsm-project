#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/IO/Probe.h>
#include <lcsm/Model/Wiring/Wire.h>

#include <stdexcept>
#include <utility>

lcsm::model::Probe::Probe() : m_wireIn(this) {}

lcsm::Identifier lcsm::model::Probe::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Probe::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wireIn.identify(m_id.next());
}

void lcsm::model::Probe::connectIn(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Probe element has only 1 input.");
	m_wireIn.connectWire(wire);
	wire->connectWire(m_wireIn);
}

void lcsm::model::Probe::connectOut(lcsm::wire_t &, std::size_t)
{
	throw std::logic_error("Probe element doesn't have any outputs.");
}

lcsm::IOComponentType lcsm::model::Probe::ioComponentType() const noexcept
{
	return lcsm::IOComponentType::IO_COMP_PROBE;
}
