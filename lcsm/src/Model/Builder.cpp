#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Tunnel.h>
#include <unordered_map>

#include <cstddef>
#include <deque>
#include <stdexcept>
#include <tuple>
#include <utility>

void lcsm::model::LCSMBuilder::addWires(const lcsm::model::Wire *wire, bool isFirstComp)
{
	const lcsm::Identifier id1 = wire->id();
	for (const lcsm::support::PointerView< lcsm::Component > &child : wire->wires())
	{
		const lcsm::model::Wire *w = static_cast< const lcsm::model::Wire * >(child.get());
		const lcsm::Identifier id2 = child->id();
		const bool isSecondComp = w->connect().hasValue();
		addWire(id1, id2, isFirstComp, isSecondComp);
	}
}

void lcsm::model::LCSMBuilder::addWire(const std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > &connect)
{
	m_wires.push_back(connect);
}

void lcsm::model::LCSMBuilder::addWire(std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > &&connect)
{
	m_wires.push_back(std::move(connect));
}

void lcsm::model::LCSMBuilder::addWire(lcsm::Identifier id1, lcsm::Identifier id2, bool isFirstComp, bool isSecondComp)
{
	m_wires.emplace_back(id1, id2, isFirstComp, isSecondComp);
}

void lcsm::model::LCSMBuilder::addWire(unsigned long long id1, unsigned long long id2, bool isFirstComp, bool isSecondComp)
{
	addWire(lcsm::Identifier(id1), lcsm::Identifier(id2), isFirstComp, isSecondComp);
}

void lcsm::model::LCSMBuilder::addTunnels(const lcsm::model::Tunnel *tunnel)
{
	const lcsm::Identifier id = tunnel->id();
	const std::vector< lcsm::Component * > &tunnels = tunnel->tunnels();
	for (lcsm::Component *child : tunnels)
	{
		const lcsm::Identifier childId = child->id();
		addTunnel(id, childId);
	}
}

void lcsm::model::LCSMBuilder::addTunnel(const std::pair< lcsm::Identifier, lcsm::Identifier > &tunnel)
{
	m_tunnels.push_back(tunnel);
}

void lcsm::model::LCSMBuilder::addTunnel(std::pair< lcsm::Identifier, lcsm::Identifier > &&tunnel)
{
	m_tunnels.push_back(std::move(tunnel));
}

void lcsm::model::LCSMBuilder::addTunnel(lcsm::Identifier id1, lcsm::Identifier id2)
{
	m_tunnels.emplace_back(id1, id2);
}

void lcsm::model::LCSMBuilder::oldToNew(lcsm::Identifier oldId, lcsm::Identifier newId)
{
	if (m_oldToNewId.find(oldId) != m_oldToNewId.cend())
	{
		throw std::logic_error("This id is already in set!");
	}
	m_oldToNewId[oldId] = newId;
}

void lcsm::model::LCSMBuilder::dump(lcsm::model::LCSMFileWriter &writer)
{
	// 'beginconnections'
	writer.writeBeginConnections();
	// 'connect <LHS> <RHS> <IS_LHS_COMP> <IS_RHS_COMP>;'
	for (const std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > &wire : m_wires)
	{
		const lcsm::Identifier lhs = std::get< 0 >(wire);
		const lcsm::Identifier rhs = std::get< 1 >(wire);
		const bool isFirstComp = std::get< 2 >(wire);
		const bool isSecondComp = std::get< 3 >(wire);
		writer.writeConnectDeclaration(lhs, rhs, isFirstComp, isSecondComp);
	}
	// 'endconnections'
	writer.writeEndConnections();
	// 'begintunnels'
	writer.writeBeginTunnels();
	// 'tunnel <LHS> <RHS>;'
	for (const std::pair< lcsm::Identifier, lcsm::Identifier > &tunnel : m_tunnels)
	{
		const lcsm::Identifier lhs = tunnel.first;
		const lcsm::Identifier rhs = tunnel.second;
		writer.writeTunnelDeclaration(lhs, rhs);
	}
	// 'endtunnels'
	writer.writeEndTunnels();
}

void lcsm::model::LCSMBuilder::finalize(lcsm::LCSMCircuit *circuit)
{
	// Make wire's connections.
	const std::size_t wiresSize = m_wires.size();
	for (std::size_t i = 0; i < wiresSize; i++)
	{
		// Extract edge.
		std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > edge = m_wires.front();
		m_wires.pop_front();

		// Find old to new identifier.
		const lcsm::Identifier wireOldId1 = std::get< 0 >(edge);
		const lcsm::Identifier wireOldId2 = std::get< 1 >(edge);
		const std::unordered_map< lcsm::Identifier, lcsm::Identifier >::const_iterator foundId1 = m_oldToNewId.find(wireOldId1);
		const std::unordered_map< lcsm::Identifier, lcsm::Identifier >::const_iterator foundId2 = m_oldToNewId.find(wireOldId2);
		if (foundId1 == m_oldToNewId.cend() || foundId2 == m_oldToNewId.cend())
		{
			m_wires.push_back(std::move(edge));
			continue;
		}

		// Try to find in circuit.
		const lcsm::Identifier wireId1 = foundId1->second;
		const lcsm::Identifier wireId2 = foundId2->second;
		const bool isFirstComp = std::get< 2 >(edge);
		const bool isSecondComp = std::get< 3 >(edge);
		lcsm::model::Wire *wire1 = nullptr;
		lcsm::model::Wire *wire2 = nullptr;

		if (isFirstComp)
		{
			wire1 = static_cast< lcsm::model::Wire * >(circuit->findGloballyComponentWire(wireId1));
		}
		else
		{
			wire1 = static_cast< lcsm::model::Wire * >(circuit->find(wireId1));
		}

		if (isSecondComp)
		{
			wire2 = static_cast< lcsm::model::Wire * >(circuit->findGloballyComponentWire(wireId2));
		}
		else
		{
			wire2 = static_cast< lcsm::model::Wire * >(circuit->find(wireId2));
		}

		if (wire1 == nullptr || wire2 == nullptr)
		{
			m_wires.push_back(std::move(edge));
			continue;
		}

		// Make connect.
		wire1->connectToWire(wire2);
	}

	// Make tunnel's connections.
	const std::size_t tunnelSizes = m_tunnels.size();
	for (std::size_t i = 0; i < tunnelSizes; i++)
	{
		// Extract edge.
		std::pair< lcsm::Identifier, lcsm::Identifier > edge = m_tunnels.front();
		m_tunnels.pop_front();

		// Find old to new identifier.
		const lcsm::Identifier tunnelOldId1 = std::get< 0 >(edge);
		const lcsm::Identifier tunnelOldId2 = std::get< 1 >(edge);
		const std::unordered_map< lcsm::Identifier, lcsm::Identifier >::const_iterator foundId1 = m_oldToNewId.find(tunnelOldId1);
		const std::unordered_map< lcsm::Identifier, lcsm::Identifier >::const_iterator foundId2 = m_oldToNewId.find(tunnelOldId2);
		if (foundId1 == m_oldToNewId.cend() || foundId2 == m_oldToNewId.cend())
		{
			m_tunnels.push_back(std::move(edge));
			continue;
		}

		// Try to find tunnels.
		const lcsm::Identifier tunnelId1 = foundId1->second;
		const lcsm::Identifier tunnelId2 = foundId2->second;
		lcsm::model::Tunnel *tunnel1 = static_cast< lcsm::model::Tunnel * >(circuit->find(tunnelId1));
		lcsm::model::Tunnel *tunnel2 = static_cast< lcsm::model::Tunnel * >(circuit->find(tunnelId2));
		if (tunnel1 == nullptr || tunnel2 == nullptr)
		{
			m_tunnels.push_back(std::move(edge));
			continue;
		}

		// Make connect.
		tunnel1->connectTunnel(tunnel2);
		tunnel2->connectTunnel(tunnel1);
	}
}

void lcsm::model::LCSMBuilder::check()
{
	if (!m_tunnels.empty() || !m_wires.empty())
	{
		throw std::logic_error("Found non empty tunnels and wires");
	}
}
