#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Wiring/Tunnel.h>
#include <lcsm/Model/Wiring/Wire.h>

#include <stdexcept>
#include <utility>

lcsm::model::Tunnel::Tunnel(const lcsm::model::Tunnel &other) : m_tunnel(other.m_tunnel) {}

lcsm::model::Tunnel::Tunnel(lcsm::model::Tunnel &&other) noexcept : m_tunnel(std::move(other.m_tunnel)) {}

lcsm::model::Tunnel &lcsm::model::Tunnel::operator=(const lcsm::model::Tunnel &other)
{
	if (this != &other)
		lcsm::model::Tunnel(other).swap(*this);
	return *this;
}

lcsm::model::Tunnel &lcsm::model::Tunnel::operator=(lcsm::model::Tunnel &&other) noexcept
{
	if (this != &other)
		lcsm::model::Tunnel(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Tunnel::swap(lcsm::model::Tunnel &other) noexcept
{
	std::swap(m_tunnel, other.m_tunnel);
}

lcsm::Identifier lcsm::model::Tunnel::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Tunnel::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wire.identify(m_id.next());
}

void lcsm::model::Tunnel::connectIn(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Tunnel element has only 1 inout.");
	connectWire(wire);
}

void lcsm::model::Tunnel::connectOut(lcsm::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Tunnel element has only 1 inout.");
	connectWire(wire);
}

void lcsm::model::Tunnel::connectWire(const lcsm::wire_t &wire)
{
	m_wire.connectWire(wire);
}

void lcsm::model::Tunnel::connectWire(lcsm::wire_t &&wire)
{
	m_wire.connectWire(std::move(wire));
}

void lcsm::model::Tunnel::connect(const lcsm::tunnel_t &tunnel) noexcept
{
	m_tunnel = tunnel;
}

void lcsm::model::Tunnel::connect(lcsm::tunnel_t &&tunnel) noexcept
{
	m_tunnel = std::move(tunnel);
}

lcsm::WiringComponentType lcsm::model::Tunnel::wiringComponentType() const noexcept
{
	return lcsm::WiringComponentType::WIRING_COMP_TUNNEL;
}

lcsm::model::Tunnel *lcsm::model::Tunnel::asTunnel() noexcept
{
	return this;
}

const lcsm::model::Tunnel *lcsm::model::Tunnel::asTunnel() const noexcept
{
	return this;
}
