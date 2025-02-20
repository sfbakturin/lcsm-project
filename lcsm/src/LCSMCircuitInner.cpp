#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <unordered_map>

#include <memory>

lcsm::LCSMCircuit::LCSMCircuitInner::LCSMCircuitInner(lcsm::Identifier circuitId, const std::shared_ptr< lcsm::LCSMCircuit > &circuit) noexcept
	: m_circuitId(circuitId), m_circuit(circuit)
{
}

bool lcsm::LCSMCircuit::LCSMCircuitInner::empty() const noexcept
{
	return !m_circuit;
}

lcsm::Identifier lcsm::LCSMCircuit::LCSMCircuitInner::circuitId() const noexcept
{
	return m_circuitId;
}

const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::LCSMCircuitInner::inputs() const noexcept
{
	return m_circuit->inputs();
}

const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::LCSMCircuitInner::outputs() const noexcept
{
	return m_circuit->outputs();
}
