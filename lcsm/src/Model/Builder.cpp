#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Tunnel.h>

#include <tuple>
#include <utility>
#include <vector>

void lcsm::model::LCSMBuilder::addWires(const lcsm::model::Wire *wire, bool isFirstComp)
{
	const lcsm::Identifier id1 = wire->id();
	for (const lcsm::support::PointerView< lcsm::Circuit > &child : wire->wires())
	{
		const lcsm::model::Wire *w = static_cast< const lcsm::model::Wire * >(child.get());
		const lcsm::Identifier id2 = child->id();
		const bool isSecondComp = w->connect().hasValue();
		m_wires.emplace_back(id1, id2, isFirstComp, isSecondComp);
	}
}

void lcsm::model::LCSMBuilder::addTunnels(const lcsm::model::Tunnel *tunnel)
{
	const lcsm::Identifier id = tunnel->id();
	const std::vector< lcsm::Circuit * > &tunnels = tunnel->tunnels();
	for (lcsm::Circuit *child : tunnels)
	{
		const lcsm::Identifier childId = child->id();
		m_tunnels.emplace_back(id, childId);
	}
}

void lcsm::model::LCSMBuilder::dumpToLCSMFile(lcsm::model::LCSMFileWriter &writer)
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
