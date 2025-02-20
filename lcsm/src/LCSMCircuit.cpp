#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Button.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Digit.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Probe.h>
#include <lcsm/Model/std/Splitter.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Model/std/TransmissionGate.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::LCSMCircuit::LCSMCircuit(lcsm::LCSMCircuit &&other) noexcept :
	m_globalId(std::move(other.m_globalId)), m_components(std::move(other.m_components))
{
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

lcsm::LCSMCircuit lcsm::LCSMCircuit::copy() const
{
	static const lcsm::Identifier entryId;
	return copyImpl(entryId);
}

const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::components() const noexcept
{
	return m_components;
}

const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::inputs() const noexcept
{
	return m_inputs;
}

const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::outputs() const noexcept
{
	return m_outputs;
}

lcsm::model::Constant *lcsm::LCSMCircuit::createConstant(lcsm::Width width, lcsm::value_t value)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Constant >(width, value);
	lcsm::model::Constant *circuit = static_cast< lcsm::model::Constant * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Ground *lcsm::LCSMCircuit::createGround(lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Ground >(width);
	lcsm::model::Ground *circuit = static_cast< lcsm::model::Ground * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Power *lcsm::LCSMCircuit::createPower(lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Power >(width);
	lcsm::model::Power *circuit = static_cast< lcsm::model::Power * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Pin *lcsm::LCSMCircuit::createPin(bool output, lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Pin >(output, width);
	lcsm::model::Pin *circuit = static_cast< lcsm::model::Pin * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Transistor *lcsm::LCSMCircuit::createTransistor(lcsm::model::Transistor::Type type)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Transistor >(type);
	lcsm::model::Transistor *circuit = static_cast< lcsm::model::Transistor * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::TransmissionGate *lcsm::LCSMCircuit::createTransmissionGate()
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::TransmissionGate >();
	lcsm::model::TransmissionGate *circuit = static_cast< lcsm::model::TransmissionGate * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Tunnel *lcsm::LCSMCircuit::createTunnel()
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Tunnel >();
	lcsm::model::Tunnel *circuit = static_cast< lcsm::model::Tunnel * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Clock *lcsm::LCSMCircuit::createClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Clock >(highDuration, lowDuration, phaseOffset);
	lcsm::model::Clock *circuit = static_cast< lcsm::model::Clock * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Button *lcsm::LCSMCircuit::createButton(bool activeOnPress)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Button >(activeOnPress);
	lcsm::model::Button *circuit = static_cast< lcsm::model::Button * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Digit *lcsm::LCSMCircuit::createDigit(bool hasDecimalPoint)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Digit >(hasDecimalPoint);
	lcsm::model::Digit *circuit = static_cast< lcsm::model::Digit * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Probe *lcsm::LCSMCircuit::createProbe()
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Probe >();
	lcsm::model::Probe *circuit = static_cast< lcsm::model::Probe * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Splitter *lcsm::LCSMCircuit::createSplitter(lcsm::Width widthIn, lcsm::width_t widthOut)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Splitter >(widthIn, widthOut);
	lcsm::model::Splitter *circuit = static_cast< lcsm::model::Splitter * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::portid_t port1, lcsm::Circuit *circuit2, lcsm::portid_t port2)
{
	// Ensure, that circuit1 and circuit2 is in this circuit.
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator found1 = m_components.begin();
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator found2 = m_components.begin();
	for (; found1 != m_components.end(); found1++)
		if (found1->second.get() == circuit1)
			break;
	for (; found2 != m_components.end(); found2++)
		if (found2->second.get() == circuit2)
			break;
	if (found1 == m_components.end() || found2 == m_components.end())
		throw std::logic_error("One of two components is not found in this circuit");
	return connectImpl(circuit1, port1, circuit2, port2);
}

lcsm::model::Wire *lcsm::LCSMCircuit::connectToInput(
	lcsm::Circuit *circuit1,
	lcsm::portid_t port1,
	const lcsm::LCSMCircuit::LCSMCircuitInner &circuit2,
	lcsm::portid_t port2,
	std::size_t iPort)
{
	// Check, if inner circuit is really inner element.
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::iterator foundCircuit =
		m_circuits.find(circuit2.circuitId());
	if (foundCircuit == m_circuits.end())
		throw std::logic_error("Provided circuit is not in this circuit.");
	// Find i-th input.
	std::shared_ptr< lcsm::LCSMCircuit > &other = foundCircuit->second;
	if (iPort > other->m_inputs.size())
		throw std::logic_error("Provided input index violates number of inputs in connection.");
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator foundComponent = other->m_inputs.begin();
	for (std::size_t i = 0; i < iPort; i++)
		foundComponent++;
	return connectImpl(circuit1, port1, foundComponent->second.get(), port2);
}

void lcsm::LCSMCircuit::remove(lcsm::Circuit *circuit)
{
	// Find this circuit, if not found - exit.
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator found = m_components.begin();
	for (; found != m_components.end(); found++)
		if (found->second.get() == circuit)
			break;
	if (found == m_components.end())
		return;
	// Remove from helpers.
	const lcsm::object_type_t objectType = circuit->objectType();
	const lcsm::Identifier id = circuit->id();
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Input))
		m_inputs.erase(id);
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Output))
		m_outputs.erase(id);
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Wiring))
		m_wires.erase(id);
	// Disconnect and remove.
	circuit->disconnectAll();
	m_components.erase(found);
}

lcsm::LCSMCircuit::LCSMCircuitInner lcsm::LCSMCircuit::addCircuit(const lcsm::LCSMCircuit &other)
{
	std::shared_ptr< lcsm::LCSMCircuit > circuit = std::make_shared< lcsm::LCSMCircuit >();
	const lcsm::Identifier circuitId = m_globalId;
	other.copyImpl(circuit.get(), m_globalId);
	m_circuits[circuitId] = circuit;
	return { circuitId, circuit };
}

lcsm::LCSMCircuit::LCSMCircuitInner lcsm::LCSMCircuit::findCircuit(lcsm::Identifier circuitId) noexcept
{
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::iterator found = m_circuits.find(circuitId);
	if (found != m_circuits.end())
	{
		const std::shared_ptr< lcsm::LCSMCircuit > &circuit = found->second;
		return { circuitId, circuit };
	}
	return {};
}

lcsm::Circuit *lcsm::LCSMCircuit::registerElement(std::shared_ptr< lcsm::Circuit > &&circuit)
{
	const std::size_t numOfWires = circuit->numOfWires();
	std::vector< std::shared_ptr< lcsm::model::Wire > > wires;
	for (std::size_t i = 0; i < numOfWires; i++)
		wires.push_back(createWire());
	circuit->provideWires(wires);
	const lcsm::Identifier prev = m_globalId;
	m_globalId = circuit->identify(prev);
	m_components[prev] = circuit;
	for (std::size_t i = 0; i < numOfWires; i++)
	{
		const lcsm::Identifier id = wires[i]->id();
		m_wires[id] = wires[i];
	}
	const lcsm::object_type_t objectType = circuit->objectType();
	const lcsm::Identifier id = circuit->id();
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Input))
		m_inputs[id] = circuit;
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Output))
		m_outputs[id] = circuit;
	return circuit.get();
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createWire()
{
	return std::make_shared< lcsm::model::Wire >();
}

void lcsm::LCSMCircuit::copyImplDfs(
	lcsm::LCSMCircuit *newCircuit,
	const lcsm::Circuit *prev,
	lcsm::Circuit *root,
	const lcsm::Circuit *next,
	std::unordered_set< lcsm::Identifier > &visited) const
{
	// Prevent infinite loop.
	const lcsm::Identifier id = next->id();
	if (visited.find(id) != visited.end())
		return;
	visited.insert(id);

	// Copy by circuit's type.
	switch (next->circuitType())
	{
	case lcsm::CircuitType::Wire:
	{
		const lcsm::model::Wire *oldWire = static_cast< const lcsm::model::Wire * >(next);

		// Special case: we copyied starting from component, so prev == root and we should just start from its wires.
		if (prev == root)
		{
			for (const lcsm::support::PointerView< lcsm::Circuit > &wire : oldWire->wires())
				copyImplDfs(newCircuit, root, nullptr, wire.cptr(), visited);
			return;
		}

		// Special case: next doesn't has connection, so there is just .
	}
	case lcsm::CircuitType::Tunnel:
	case lcsm::CircuitType::Pin:
	{
		const lcsm::model::Pin *oldPin = static_cast< const lcsm::model::Pin * >(next);
		const bool output = oldPin->output();
		const lcsm::Width width = oldPin->width();
		lcsm::model::Pin *newPin = newCircuit->createPin(output, width);
		lcsm::Circuit *newInternal = newCircuit->m_wires[newPin->internal()->id()].get();
		lcsm::Circuit *newExternal = newCircuit->m_wires[newPin->external()->id()].get();
		const lcsm::Circuit *oldInternal = oldPin->internal();
		const lcsm::Circuit *oldExternal = oldPin->external();
		copyImplDfs(newCircuit, newInternal, newInternal, oldInternal, visited);
		copyImplDfs(newCircuit, newExternal, newExternal, oldExternal, visited);
		break;
	}
	case lcsm::CircuitType::Constant:
	case lcsm::CircuitType::Power:
	case lcsm::CircuitType::Ground:
	case lcsm::CircuitType::Clock:
	case lcsm::CircuitType::Transistor:
	case lcsm::CircuitType::TransmissionGate:
	case lcsm::CircuitType::Button:
	case lcsm::CircuitType::Digit:
	case lcsm::CircuitType::Probe:
	case lcsm::CircuitType::Splitter:
		break;
	}
}

void lcsm::LCSMCircuit::copyImpl(lcsm::LCSMCircuit *newCircuit, const Identifier &entryId) const
{
	// Re-entry global Id.
	newCircuit->m_globalId = entryId;

	// Rebuilt circuit elements via DFS.
	std::unordered_set< lcsm::Identifier > visited;
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator begin = m_components.begin();
	const lcsm::Circuit *next = begin->second.get();
	copyImplDfs(newCircuit, nullptr, nullptr, next, visited);
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::copyImpl(const lcsm::Identifier &entryId) const
{
	lcsm::LCSMCircuit newCircuit;
	copyImpl(std::addressof(newCircuit), entryId);
	return newCircuit;
}

lcsm::model::Wire *
	lcsm::LCSMCircuit::connectImpl(lcsm::Circuit *circuit1, lcsm::portid_t port1, lcsm::Circuit *circuit2, lcsm::portid_t port2)
{
	// Make connect via new wire.
	std::shared_ptr< lcsm::model::Wire > wire = createWire();
	m_globalId = wire->identify(m_globalId);
	m_wires[wire->id()] = wire;
	circuit1->connect(port1, wire.get());
	circuit2->connect(port2, wire.get());
	lcsm::Circuit *wire1 = circuit1->byPort(port1);
	lcsm::Circuit *wire2 = circuit2->byPort(port2);
	wire->connectToWire(wire1);
	wire->connectToWire(wire2);
	return wire.get();
}
