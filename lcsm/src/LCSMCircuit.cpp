#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <utility>

lcsm::LCSMCircuit::LCSMCircuit(const lcsm::LCSMCircuit &other) : m_globalId(other.m_globalId)
{
	/* Rebuilt circuit elements via DFS. */
	// TODO: Implement me.
}

lcsm::LCSMCircuit::LCSMCircuit(lcsm::LCSMCircuit &&other) noexcept :
	m_globalId(std::move(other.m_globalId)), m_components(std::move(other.m_components))
{
}

lcsm::LCSMCircuit &lcsm::LCSMCircuit::operator=(const lcsm::LCSMCircuit &other)
{
	return lcsm::support::CopyAssign< lcsm::LCSMCircuit >(this, other);
}

lcsm::LCSMCircuit &lcsm::LCSMCircuit::operator=(lcsm::LCSMCircuit &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::LCSMCircuit >(this, std::move(other));
}

void lcsm::LCSMCircuit::swap(lcsm::LCSMCircuit &other) noexcept
{
	std::swap(m_globalId, other.m_globalId);
	std::swap(m_components, other.m_components);
}

const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::components() const noexcept
{
	return m_components;
}

lcsm::Circuit *lcsm::LCSMCircuit::registerElement(std::shared_ptr< lcsm::Circuit > &&circuit)
{
	const lcsm::Identifier prev = m_globalId;
	m_globalId = circuit->identify(prev);
	m_components[prev] = std::move(circuit);
	return m_components[prev].get();
}
