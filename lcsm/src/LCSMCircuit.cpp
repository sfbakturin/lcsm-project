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
#include <deque>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::LCSMCircuit::LCSMCircuit(const lcsm::LCSMCircuit &other) : m_globalId(other.m_globalId)
{
	// Rebuilt circuit elements via BFS.
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &components = other.m_components;
	std::unordered_set< lcsm::Identifier > visited;
	std::deque< lcsm::Identifier > queue;
	// Put all roots to queue.
	for (const auto &component : components)
	{
		const lcsm::object_type_t objectType = component.second->objectType();
		const lcsm::Identifier id = component.second->id();
		if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Root))
			queue.push_back(id);
	}
	// Run the algorithm from roots.
	/*lcsm::LCSMBuilder builder(*this);
	while (!queue.empty())
	{
		const lcsm::Identifier id = queue.front();
		queue.pop_front();
		if (visited.find(id) != visited.end())
			continue;
		visited.insert(id);
		const auto &found = components.find(id);
		const std::shared_ptr< lcsm::Circuit > &circuit = found->second;
		switch (circuit->circuitType())
		{
		case lcsm::CircuitType::Wire:
		{
			break;
		}
		case lcsm::CircuitType::Tunnel:
		case lcsm::CircuitType::Pin:
		{
			const lcsm::model::Pin *oldPin = static_cast< lcsm::model::Pin * >(circuit.get());
			const bool output = oldPin->output();
			const lcsm::Width width = oldPin->width();
			lcsm::model::Pin *newPin = builder.CreatePin(output, width);

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
	*/
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
