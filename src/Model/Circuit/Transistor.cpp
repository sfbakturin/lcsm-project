#include <sim/Component/Identifier.h>
#include <sim/Model/Circuit/Transistor.h>
#include <sim/Model/Wiring/Wire.h>

#include <memory>
#include <stdexcept>
#include <utility>

sim::Transistor::Transistor(TransistorType type) :
	m_type(type), m_base(this), m_inoutA(this), m_inoutB(this)
{
}

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
		sim::Transistor(other).swap(*this);
	return *this;
}

sim::Transistor &sim::Transistor::operator=(sim::Transistor &&other)
{
	if (this != &other)
		sim::Transistor(std::move(other)).swap(*this);
	return *this;
}

void sim::Transistor::swap(sim::Transistor &other) noexcept
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
	m_idInoutA = m_idBase.next();
	m_idInoutB = m_idInoutA.next();

	sim::Identifier next = m_base.identify(m_idInoutB.next());
	next = m_inoutA.identify(next);
	next = m_inoutB.identify(next);

	return next;
}

sim::Identifier sim::Transistor::idBase() const noexcept
{
	return m_idBase;
}

sim::Identifier sim::Transistor::idInoutA() const noexcept
{
	return m_idInoutA;
}

sim::Identifier sim::Transistor::idInoutB() const noexcept
{
	return m_idInoutB;
}

void sim::Transistor::connectIn(sim::wire_t &wire, std::size_t i)
{
	connect(wire, i);
}

void sim::Transistor::connectOut(sim::wire_t &wire, std::size_t i)
{
	connect(wire, i);
}

void sim::Transistor::connectBase(sim::wire_t &wire)
{
	connectIn(wire, 0);
}

void sim::Transistor::connectInoutA(sim::wire_t &wire)
{
	connectOut(wire, 1);
}

void sim::Transistor::connectInoutB(sim::wire_t &wire)
{
	connectOut(wire, 2);
}

std::size_t sim::Transistor::testConnectivity(const Wire *wire) const noexcept
{
	if (testConnectivityBase(wire))
		return sim::Transistor::CompositeIndex::BASE;
	else if (testConnectivityInoutA(wire))
		return sim::Transistor::CompositeIndex::INOUT_A;
	else if (testConnectivityInoutB(wire))
		return sim::Transistor::CompositeIndex::INOUT_B;
	else
		return -1;
}

bool sim::Transistor::testConnectivityBase(const sim::Wire *wire) const noexcept
{
	return std::addressof(m_base) == wire;
}

bool sim::Transistor::testConnectivityInoutA(const sim::Wire *wire) const noexcept
{
	return std::addressof(m_inoutA) == wire;
}

bool sim::Transistor::testConnectivityInoutB(const sim::Wire *wire) const noexcept
{
	return std::addressof(m_inoutB) == wire;
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

sim::Wire &sim::Transistor::wireInoutA() noexcept
{
	return m_inoutA;
}

const sim::Wire &sim::Transistor::wireInoutA() const noexcept
{
	return m_inoutA;
}

sim::Wire &sim::Transistor::wireInoutB() noexcept
{
	return m_inoutB;
}

const sim::Wire &sim::Transistor::wireInoutB() const noexcept
{
	return m_inoutB;
}

void sim::Transistor::connect(sim::wire_t &wire, std::size_t i)
{
	sim::Wire *selected = nullptr;

	switch (i)
	{
	case sim::Transistor::CompositeIndex::BASE:
		selected = std::addressof(m_base);
		break;
	case sim::Transistor::CompositeIndex::INOUT_A:
		selected = std::addressof(m_inoutA);
		break;
	case sim::Transistor::CompositeIndex::INOUT_B:
		selected = std::addressof(m_inoutB);
		break;
	default:
		throw std::logic_error(
			"Transistor element has BASE and 2 INOUT "
			"connections.");
	}

	selected->connectWire(wire);
	wire->connectWire(selected);
}
