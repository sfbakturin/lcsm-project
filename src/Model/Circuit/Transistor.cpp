#include <sim/Model/Circuit/Transistor.h>

#include <stdexcept>
#include <utility>

sim::Transistor::Transistor(TransistorType type) : m_id(0), m_type(type) {}

sim::Transistor::Transistor(const sim::Transistor &other) : m_type(other.m_type)
{
}

sim::Transistor::Transistor(sim::Transistor &&other) noexcept :
	m_type(other.m_type)
{
}

sim::Transistor &sim::Transistor::operator=(const sim::Transistor &other)
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

unsigned sim::Transistor::ID() const noexcept
{
	return m_id;
}

void sim::Transistor::Identify(unsigned ID) noexcept
{
	m_id = ID;
}

void sim::Transistor::ConnectIn(sim::wire_t &wire, std::size_t i)
{
	Connect(wire, i);
}

void sim::Transistor::ConnectOut(sim::wire_t &wire, std::size_t i)
{
	Connect(wire, i);
}

void sim::Transistor::ConnectBase(sim::wire_t &wire)
{
	ConnectIn(wire, 0);
}

void sim::Transistor::ConnectSrcA(sim::wire_t &wire)
{
	ConnectOut(wire, 1);
}

void sim::Transistor::ConnectSrcB(sim::wire_t &wire)
{
	ConnectOut(wire, 2);
}

sim::CircuitComponentType sim::Transistor::circuitComponentType() const noexcept
{
	return sim::CircuitComponentType::CIRCUIT_COMP_TRANSISTOR;
}

sim::Transistor *sim::Transistor::asTransistor() noexcept
{
	return this;
}

const sim::Transistor *sim::Transistor::asTransistor() const noexcept
{
	return this;
}

sim::Wire &sim::Transistor::wireBase() noexcept
{
	return m_base;
}

const sim::Wire &sim::Transistor::wireBase() const noexcept
{
	return m_base;
}

sim::Wire &sim::Transistor::wireSrcA() noexcept
{
	return m_srcs[0];
}

const sim::Wire &sim::Transistor::wireSrcA() const noexcept
{
	return m_srcs[0];
}

sim::Wire &sim::Transistor::wireSrcB() noexcept
{
	return m_srcs[1];
}

const sim::Wire &sim::Transistor::wireSrcB() const noexcept
{
	return m_srcs[1];
}

void sim::Transistor::Connect(sim::wire_t &wire, std::size_t i)
{
	if (i == 0)
		m_base.ConnectWire(wire);
	else if (i - 1 < sim::Transistor::SRC_N)
		m_srcs[i - 1].ConnectWire(wire);
	throw std::logic_error("Transistor element has only 3 inout connections.");
}
