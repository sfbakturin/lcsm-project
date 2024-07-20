#include <sim/Model/Circuit/Transistor.h>

#include <stdexcept>
#include <utility>

sim::Transistor::Transistor() noexcept : m_type(sim::TransistorType::P) {}
sim::Transistor::Transistor(TransistorType type) noexcept : m_type(type) {}

sim::Transistor::Transistor(const sim::Transistor &other) noexcept : m_type(other.m_type) {}
sim::Transistor::Transistor(sim::Transistor &&other) noexcept : m_type(other.m_type) {}

sim::Transistor &sim::Transistor::operator=(const sim::Transistor &other) noexcept
{
	if (this != &other)
		sim::Transistor(other).Swap(*this);
	return *this;
}
sim::Transistor &sim::Transistor::operator=(sim::Transistor &&other)
{
	if (this != &other)
		sim::Transistor(std::move(other)).Swap(*this);
	return *this;
}

void sim::Transistor::Swap(sim::Transistor &other) noexcept
{
	std::swap(m_type, other.m_type);
}

void sim::Transistor::ConnectIn(const sim::wire_t &wire, std::size_t i)
{
	Connect(wire, i);
}
void sim::Transistor::ConnectOut(const sim::wire_t &wire, std::size_t i)
{
	Connect(wire, i);
}

void sim::Transistor::ConnectBase(const sim::wire_t &wire)
{
	ConnectIn(wire, 0);
}
void sim::Transistor::ConnectSrcA(const sim::wire_t &wire)
{
	ConnectOut(wire, 1);
}
void sim::Transistor::ConnectSrcB(const sim::wire_t &wire)
{
	ConnectOut(wire, 2);
}

void sim::Transistor::Connect(const sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
		m_base.ConnectWire(wire);
	else if (i - 1 < sim::Transistor::SRC_N)
		m_srcs[i].ConnectWire(wire);
	throw std::logic_error("Transistor element has only 3 inout connections.");
}
