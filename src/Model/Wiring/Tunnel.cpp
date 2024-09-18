#include <sim/Component/Identifier.h>
#include <sim/Model/Wiring/Tunnel.h>
#include <sim/Model/Wiring/Wire.h>

#include <stdexcept>
#include <utility>

sim::Tunnel::Tunnel(const sim::Tunnel &other) : m_tunnel(other.m_tunnel) {}

sim::Tunnel::Tunnel(sim::Tunnel &&other) noexcept :
	m_tunnel(std::move(other.m_tunnel))
{
}

sim::Tunnel &sim::Tunnel::operator=(const sim::Tunnel &other)
{
	if (this != &other)
		sim::Tunnel(other).Swap(*this);
	return *this;
}

sim::Tunnel &sim::Tunnel::operator=(sim::Tunnel &&other) noexcept
{
	if (this != &other)
		sim::Tunnel(std::move(other)).Swap(*this);
	return *this;
}

void sim::Tunnel::Swap(sim::Tunnel &other) noexcept
{
	std::swap(m_tunnel, other.m_tunnel);
}

sim::Identifier sim::Tunnel::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Tunnel::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	return m_wire.identify(m_id.next());
}

void sim::Tunnel::connectIn(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Tunnel element has only 1 inout.");
	connectWire(wire);
}

void sim::Tunnel::connectOut(sim::wire_t &wire, std::size_t i)
{
	if (i != 0)
		throw std::logic_error("Tunnel element has only 1 inout.");
	connectWire(wire);
}

void sim::Tunnel::connectWire(const sim::wire_t &wire)
{
	m_wire.connectWire(wire);
}

void sim::Tunnel::connectWire(sim::wire_t &&wire)
{
	m_wire.connectWire(std::move(wire));
}

void sim::Tunnel::connect(const sim::tunnel_t &tunnel) noexcept
{
	m_tunnel = tunnel;
}

void sim::Tunnel::connect(sim::tunnel_t &&tunnel) noexcept
{
	m_tunnel = std::move(tunnel);
}

sim::WiringComponentType sim::Tunnel::wiringComponentType() const noexcept
{
	return sim::WiringComponentType::WIRING_COMP_TUNNEL;
}

sim::Tunnel *sim::Tunnel::asTunnel() noexcept
{
	return this;
}

const sim::Tunnel *sim::Tunnel::asTunnel() const noexcept
{
	return this;
}
