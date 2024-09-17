#include <sim/Component/Identifier.h>
#include <sim/Model/Circuit/Transistor.h>

#include <memory>
#include <stdexcept>
#include <utility>

sim::Transistor::Transistor(TransistorType type) : m_type(type) {}

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

sim::Identifier sim::Transistor::ID() const noexcept
{
	return m_id;
}

sim::Identifier sim::Transistor::identify(sim::Identifier ID) noexcept
{
	m_id = std::move(ID);
	m_idBase = m_id.next();
	m_idSrcA = m_idBase.next();
	m_idSrcB = m_idSrcA.next();
	sim::Identifier next = m_base.identify(m_idSrcB.next());
	for (std::size_t i = 0; i < sim::Transistor::SRC_N; i++)
		next = m_srcs[i].identify(next.next());
	return next;
}

sim::Identifier sim::Transistor::idBase() const noexcept
{
	return m_idBase;
}

sim::Identifier sim::Transistor::idSrcA() const noexcept
{
	return m_idSrcA;
}

sim::Identifier sim::Transistor::idSrcB() const noexcept
{
	return m_idSrcB;
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

bool sim::Transistor::testConnectivityBase(const sim::Wire *wire) const noexcept
{
	return std::addressof(m_base) == wire;
}

bool sim::Transistor::testConnectivitySrcA(const sim::Wire *wire) const noexcept
{
	return std::addressof(m_srcs[0]) == wire;
}

bool sim::Transistor::testConnectivitySrcB(const sim::Wire *wire) const noexcept
{
	return std::addressof(m_srcs[1]) == wire;
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
