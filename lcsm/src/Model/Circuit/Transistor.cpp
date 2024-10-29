#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Circuit/Transistor.h>
#include <lcsm/Model/Wiring/Wire.h>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Transistor::Transistor(TransistorType type) : m_type(type), m_base(this), m_inoutA(this), m_inoutB(this) {}

lcsm::model::Transistor::Transistor(const lcsm::model::Transistor &other) : m_type(other.m_type) {}

lcsm::model::Transistor::Transistor(lcsm::model::Transistor &&other) noexcept : m_type(other.m_type) {}

lcsm::model::Transistor &lcsm::model::Transistor::operator=(const lcsm::model::Transistor &other)
{
	if (this != &other)
		lcsm::model::Transistor(other).swap(*this);
	return *this;
}

lcsm::model::Transistor &lcsm::model::Transistor::operator=(lcsm::model::Transistor &&other)
{
	if (this != &other)
		lcsm::model::Transistor(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Transistor::swap(lcsm::model::Transistor &other) noexcept
{
	std::swap(m_type, other.m_type);
}

lcsm::Identifier lcsm::model::Transistor::ID() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Transistor::identify(lcsm::Identifier ID) noexcept
{
	m_id = std::move(ID);
	m_idBase = m_id.next();
	m_idInoutA = m_idBase.next();
	m_idInoutB = m_idInoutA.next();

	lcsm::Identifier next = m_base.identify(m_idInoutB.next());
	next = m_inoutA.identify(next);
	next = m_inoutB.identify(next);

	return next;
}

lcsm::Identifier lcsm::model::Transistor::idBase() const noexcept
{
	return m_idBase;
}

lcsm::Identifier lcsm::model::Transistor::idInoutA() const noexcept
{
	return m_idInoutA;
}

lcsm::Identifier lcsm::model::Transistor::idInoutB() const noexcept
{
	return m_idInoutB;
}

void lcsm::model::Transistor::connectIn(lcsm::wire_t &wire, std::size_t i)
{
	connect(wire, i);
}

void lcsm::model::Transistor::connectOut(lcsm::wire_t &wire, std::size_t i)
{
	connect(wire, i);
}

void lcsm::model::Transistor::connectBase(lcsm::wire_t &wire)
{
	connectIn(wire, 0);
}

void lcsm::model::Transistor::connectInoutA(lcsm::wire_t &wire)
{
	connectOut(wire, 1);
}

void lcsm::model::Transistor::connectInoutB(lcsm::wire_t &wire)
{
	connectOut(wire, 2);
}

std::size_t lcsm::model::Transistor::testConnectivity(const Wire *wire) const noexcept
{
	if (testConnectivityBase(wire))
		return lcsm::model::Transistor::CompositeIndex::BASE;
	else if (testConnectivityInoutA(wire))
		return lcsm::model::Transistor::CompositeIndex::INOUT_A;
	else if (testConnectivityInoutB(wire))
		return lcsm::model::Transistor::CompositeIndex::INOUT_B;
	else
		return -1;
}

bool lcsm::model::Transistor::testConnectivityBase(const lcsm::model::Wire *wire) const noexcept
{
	return std::addressof(m_base) == wire;
}

bool lcsm::model::Transistor::testConnectivityInoutA(const lcsm::model::Wire *wire) const noexcept
{
	return std::addressof(m_inoutA) == wire;
}

bool lcsm::model::Transistor::testConnectivityInoutB(const lcsm::model::Wire *wire) const noexcept
{
	return std::addressof(m_inoutB) == wire;
}

lcsm::CircuitComponentType lcsm::model::Transistor::circuitComponentType() const noexcept
{
	return lcsm::CircuitComponentType::CIRCUIT_COMP_TRANSISTOR;
}

lcsm::model::Transistor *lcsm::model::Transistor::asTransistor() noexcept
{
	return this;
}

const lcsm::model::Transistor *lcsm::model::Transistor::asTransistor() const noexcept
{
	return this;
}

lcsm::model::Wire &lcsm::model::Transistor::wireBase() noexcept
{
	return m_base;
}

const lcsm::model::Wire &lcsm::model::Transistor::wireBase() const noexcept
{
	return m_base;
}

lcsm::model::Wire &lcsm::model::Transistor::wireInoutA() noexcept
{
	return m_inoutA;
}

const lcsm::model::Wire &lcsm::model::Transistor::wireInoutA() const noexcept
{
	return m_inoutA;
}

lcsm::model::Wire &lcsm::model::Transistor::wireInoutB() noexcept
{
	return m_inoutB;
}

const lcsm::model::Wire &lcsm::model::Transistor::wireInoutB() const noexcept
{
	return m_inoutB;
}

void lcsm::model::Transistor::connect(lcsm::wire_t &wire, std::size_t i)
{
	lcsm::model::Wire *selected = nullptr;

	switch (i)
	{
	case lcsm::model::Transistor::CompositeIndex::BASE:
		selected = std::addressof(m_base);
		break;
	case lcsm::model::Transistor::CompositeIndex::INOUT_A:
		selected = std::addressof(m_inoutA);
		break;
	case lcsm::model::Transistor::CompositeIndex::INOUT_B:
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
