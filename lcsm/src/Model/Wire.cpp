#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

#include <stdexcept>
#include <utility>
#include <vector>

lcsm::Identifier lcsm::model::Wire::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Wire::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_id.next();
}

lcsm::ObjectType lcsm::model::Wire::objectType() const noexcept
{
	return lcsm::ObjectType::PureInt;
}

lcsm::CircuitType lcsm::model::Wire::circuitType() const noexcept
{
	return lcsm::CircuitType::Wire;
}

void lcsm::model::Wire::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Wire::Port pw = static_cast< lcsm::model::Wire::Port >(portId);

	/* If port is Wiring, then connect as wire, otherwise as connector. */
	// TODO: There is should be somewhat implemented error handling, when on Wiring somebody tries connect not-wire. */
	switch (pw)
	{
	case lcsm::model::Wire::Port::Wiring:
	{
		m_wires.push_back(circuit);
		break;
	}
	case lcsm::model::Wire::Port::Connect:
	{
		m_connect = circuit;
		break;
	}
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Wire::connectToWire(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Wire::Port::Wiring, circuit);
}

void lcsm::model::Wire::connectConnect(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Wire::Port::Connect, circuit);
}

const std::vector< lcsm::support::PointerView< lcsm::Circuit > > &lcsm::model::Wire::wires() const noexcept
{
	return m_wires;
}

const lcsm::support::PointerView< lcsm::Circuit > &lcsm::model::Wire::connect() const noexcept
{
	return m_connect;
}

// lcsm::model::Wire::Wire(lcsm::Component *comp)
// {
// 	connect(lcsm::support::PointerView< lcsm::Component >(comp));
// }

// lcsm::Identifier lcsm::model::Wire::ID() const noexcept
// {
// 	return m_id;
// }

// lcsm::Identifier lcsm::model::Wire::identify(lcsm::Identifier ID) noexcept
// {
// 	m_id = std::move(ID);
// 	return m_id.next();
// }

// void lcsm::model::Wire::connectIn(lcsm::wire_t &wire, std::size_t i)
// {
// 	if (i != 0)
// 		throw std::logic_error("");
// 	connectWire(wire);
// }

// void lcsm::model::Wire::connectOut(lcsm::wire_t &wire, std::size_t i)
// {
// 	if (i != 0)
// 		throw std::logic_error("");
// 	connectWire(wire);
// }

// void lcsm::model::Wire::connectWire(const lcsm::wire_t &wire)
// {
// 	m_wires.push_back(wire);
// }

// void lcsm::model::Wire::connectWire(lcsm::wire_t &&wire)
// {
// 	m_wires.push_back(std::move(wire));
// }

// void lcsm::model::Wire::connect(const lcsm::component_t &connect)
// {
// 	if (connect->isWiring())
// 		throw std::logic_error(
// 			"All wiring components should be connected via "
// 			"ConnectWire method.");
// 	m_connections.push_back(connect);
// }

// void lcsm::model::Wire::connect(lcsm::component_t &&connect)
// {
// 	if (connect->isWiring())
// 		throw std::logic_error(
// 			"All wiring components should be connected via "
// 			"ConnectWire method.");
// 	m_connections.push_back(std::move(connect));
// }

// const std::vector< lcsm::wire_t > &lcsm::model::Wire::wires() const noexcept
// {
// 	return m_wires;
// }

// const std::vector< lcsm::component_t > &lcsm::model::Wire::connections() const noexcept
// {
// 	return m_connections;
// }

// lcsm::WiringComponentType lcsm::model::Wire::wiringComponentType() const noexcept
// {
// 	return lcsm::WiringComponentType::WIRING_COMP_WIRE;
// }

// lcsm::model::Wire *lcsm::model::Wire::asWire() noexcept
// {
// 	return this;
// }

// const lcsm::model::Wire *lcsm::model::Wire::asWire() const noexcept
// {
// 	return this;
// }
