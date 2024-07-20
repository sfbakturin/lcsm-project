#include <sim/Model/Wiring/Tunnel.h>
#include <sim/Model/Wiring/Wire.h>

#include <utility>

sim::Tunnel::Tunnel(const sim::Tunnel &other) : m_tunnel(other.m_tunnel) {}
sim::Tunnel::Tunnel(sim::Tunnel &&other) noexcept : m_tunnel(std::move(other.m_tunnel)) {}

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

void sim::Tunnel::ConnectWire(const sim::wire_t &wire)
{
	m_wire.ConnectWire(wire);
}
void sim::Tunnel::ConnectWire(sim::wire_t &&wire)
{
	m_wire.ConnectWire(std::move(wire));
}

void sim::Tunnel::Connect(const sim::tunnel_t &tunnel) noexcept
{
	m_tunnel = tunnel;
}
void sim::Tunnel::Connect(sim::tunnel_t &&tunnel) noexcept
{
	m_tunnel = std::move(tunnel);
}
