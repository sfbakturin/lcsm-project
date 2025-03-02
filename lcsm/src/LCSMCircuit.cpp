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
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Model/std/Probe.h>
#include <lcsm/Model/std/Splitter.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Model/std/TransmissionGate.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::LCSMCircuit::LCSMCircuit(lcsm::label_t name) : m_name(name) {}

lcsm::LCSMCircuit::LCSMCircuit(lcsm::LCSMCircuit &&other) noexcept :
	m_globalId(std::move(other.m_globalId)), m_components(std::move(other.m_components)),
	m_inputs(std::move(other.m_inputs)), m_outputs(std::move(other.m_outputs)),
	m_componentWires(std::move(other.m_componentWires)), m_connectorWires(std::move(other.m_connectorWires)),
	m_circuits(std::move(other.m_circuits)), m_name(std::move(other.m_name))
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
	std::swap(m_inputs, other.m_inputs);
	std::swap(m_outputs, other.m_outputs);
	std::swap(m_componentWires, other.m_componentWires);
	std::swap(m_connectorWires, other.m_connectorWires);
	std::swap(m_circuits, other.m_circuits);
	std::swap(m_name, other.m_name);
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::copy() const
{
	static const lcsm::Identifier entryId;
	return copyImpl(entryId);
}

lcsm::Identifier lcsm::LCSMCircuit::globalId() const noexcept
{
	return m_globalId;
}

const std::string &lcsm::LCSMCircuit::name() const noexcept
{
	return m_name;
}

lcsm::label_t lcsm::LCSMCircuit::c_name() const noexcept
{
	return m_name.c_str();
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::components() const noexcept
{
	return m_components;
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::inputs() const noexcept
{
	return m_inputs;
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::outputs() const noexcept
{
	return m_outputs;
}

lcsm::model::Constant *lcsm::LCSMCircuit::createConstant(lcsm::label_t name, lcsm::Width width, lcsm::value_t value)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Constant >(name, width, value);
	lcsm::model::Constant *circuit = static_cast< lcsm::model::Constant * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Ground *lcsm::LCSMCircuit::createGround(lcsm::label_t name, lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Ground >(name, width);
	lcsm::model::Ground *circuit = static_cast< lcsm::model::Ground * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Power *lcsm::LCSMCircuit::createPower(lcsm::label_t name, lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Power >(name, width);
	lcsm::model::Power *circuit = static_cast< lcsm::model::Power * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Pin *lcsm::LCSMCircuit::createPin(bool output, lcsm::label_t name, lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Pin >(name, output, width);
	lcsm::model::Pin *circuit = static_cast< lcsm::model::Pin * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Transistor *lcsm::LCSMCircuit::createTransistor(lcsm::label_t name, lcsm::model::Transistor::Type type)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Transistor >(name, type);
	lcsm::model::Transistor *circuit = static_cast< lcsm::model::Transistor * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::TransmissionGate *lcsm::LCSMCircuit::createTransmissionGate(lcsm::label_t name)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::TransmissionGate >(name);
	lcsm::model::TransmissionGate *circuit = static_cast< lcsm::model::TransmissionGate * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Tunnel *lcsm::LCSMCircuit::createTunnel(lcsm::label_t name)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Tunnel >(name);
	lcsm::model::Tunnel *circuit = static_cast< lcsm::model::Tunnel * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Clock *lcsm::LCSMCircuit::createClock(lcsm::label_t name, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Clock >(name, highDuration, lowDuration, phaseOffset);
	lcsm::model::Clock *circuit = static_cast< lcsm::model::Clock * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Button *lcsm::LCSMCircuit::createButton(lcsm::label_t name, bool activeOnPress)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Button >(name, activeOnPress);
	lcsm::model::Button *circuit = static_cast< lcsm::model::Button * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Digit *lcsm::LCSMCircuit::createDigit(lcsm::label_t name, bool hasDecimalPoint)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Digit >(name, hasDecimalPoint);
	lcsm::model::Digit *circuit = static_cast< lcsm::model::Digit * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Probe *lcsm::LCSMCircuit::createProbe(lcsm::label_t name)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Probe >(name);
	lcsm::model::Probe *circuit = static_cast< lcsm::model::Probe * >(registerElement(std::move(element)));
	return circuit;
}

lcsm::model::Splitter *lcsm::LCSMCircuit::createSplitter(lcsm::label_t name, lcsm::Width widthIn, lcsm::width_t widthOut)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Splitter >(name, widthIn, widthOut);
	lcsm::model::Splitter *circuit = static_cast< lcsm::model::Splitter * >(registerElement(std::move(element)));
	return circuit;
}

static std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator
	FindCircuit(const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &m, lcsm::Circuit *c)
{
	return std::find_if(
		m.begin(),
		m.end(),
		[c](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i) { return i.second.get() == c; });
}

lcsm::model::Wire *
	lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::portid_t port1, lcsm::Circuit *circuit2, lcsm::portid_t port2, lcsm::label_t name)
{
	// If circuit1 or circuit2 is nullptrs, then return nullptr - as not succeeded.
	if (circuit1 == nullptr || circuit2 == nullptr)
		return nullptr;

	// Ensure, that circuit1 and circuit2 is in this circuit.
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator end = m_components.cend();

	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundAsComponent1 =
		FindCircuit(m_components, circuit1);
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundAsConnectorWire1 =
		FindCircuit(m_connectorWires, circuit1);

	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundAsInputInCircuit1 = end;
	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &lcsmCircuit : m_circuits)
	{
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &inputsInLcsmCircuit = lcsmCircuit.second->inputs();
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &outputsInLcsmCircuit = lcsmCircuit.second->outputs();

		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundInInputs =
			FindCircuit(inputsInLcsmCircuit, circuit1);
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundInOutputs =
			FindCircuit(outputsInLcsmCircuit, circuit1);

		if (foundInInputs != inputsInLcsmCircuit.end())
		{
			foundAsInputInCircuit1 = foundInInputs;
			break;
		}

		if (foundInOutputs != outputsInLcsmCircuit.end())
		{
			foundAsInputInCircuit1 = foundInOutputs;
			break;
		}
	}

	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundAsComponent2 =
		FindCircuit(m_components, circuit2);
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundAsConnectorWire2 =
		FindCircuit(m_connectorWires, circuit2);

	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundAsInputInCircuit2 = end;
	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &lcsmCircuit : m_circuits)
	{
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &inputsInLcsmCircuit = lcsmCircuit.second->inputs();
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &outputsInLcsmCircuit = lcsmCircuit.second->outputs();

		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundInInputs =
			FindCircuit(inputsInLcsmCircuit, circuit1);
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator foundInOutputs =
			FindCircuit(outputsInLcsmCircuit, circuit1);

		if (foundInInputs != inputsInLcsmCircuit.end())
		{
			foundAsInputInCircuit2 = foundInInputs;
			break;
		}

		if (foundInOutputs != outputsInLcsmCircuit.end())
		{
			foundAsInputInCircuit2 = foundInOutputs;
			break;
		}
	}

	if ((foundAsComponent1 == end && foundAsConnectorWire1 == end && foundAsInputInCircuit1 == end) ||
		(foundAsComponent2 == end && foundAsConnectorWire2 == end && foundAsInputInCircuit2 == end))
		throw std::logic_error("One of two components is not found in this circuit");

	// Make connect via new wire.
	std::shared_ptr< lcsm::model::Wire > wire = createConnectorWire(name);
	circuit1->connect(port1, wire.get());
	circuit2->connect(port2, wire.get());
	lcsm::Circuit *wire1 = circuit1->byPort(port1);
	lcsm::Circuit *wire2 = circuit2->byPort(port2);
	wire->connectToWire(wire1);
	wire->connectToWire(wire2);
	return wire.get();
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::Circuit *circuit2, lcsm::portid_t port2, lcsm::label_t name)
{
	// Use first circuit's default port, when `circuit1` is not nullptr.
	if (circuit1 == nullptr)
		return nullptr;
	return connect(circuit1, circuit1->defaultPort(), circuit2, port2, name);
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::portid_t port1, lcsm::Circuit *circuit2, lcsm::label_t name)
{
	// Use second circuit's default port, when `circuit2` is not nullptr.
	if (circuit2 == nullptr)
		return nullptr;
	return connect(circuit1, port1, circuit2, circuit2->defaultPort(), name);
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::Circuit *circuit2, lcsm::label_t name)
{
	// Use default ports, when `circuit1` and `circuit2` are not nullptr.
	if (circuit1 == nullptr || circuit2 == nullptr)
		return nullptr;
	return connect(circuit1, circuit1->defaultPort(), circuit2, circuit2->defaultPort(), name);
}

lcsm::Circuit *lcsm::LCSMCircuit::find(lcsm::Identifier id) noexcept
{
	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator found = m_components.find(id);
	if (found == m_components.end())
		return nullptr;
	return found->second.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::find(lcsm::label_t name) noexcept
{
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator found = std::find_if(
		m_components.begin(),
		m_components.end(),
		[name](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i)
		{ return std::strcmp(i.second->c_name(), name) == 0; });
	if (found == m_components.end())
		return nullptr;
	return found->second.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::find(const std::string &name) noexcept
{
	return find(name.c_str());
}

bool lcsm::LCSMCircuit::remove(lcsm::Circuit *circuit)
{
	// If `circuit` is nullptr, do nothing.
	if (circuit == nullptr)
	{
		return false;
	}

	// Find this circuit, if not found - exit.
	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator found = m_components.begin();
	for (; found != m_components.end(); found++)
	{
		if (found->second.get() == circuit)
		{
			break;
		}
	}
	if (found == m_components.end())
	{
		return false;
	}

	// Remove from helpers.
	const lcsm::object_type_t objectType = circuit->objectType();
	const lcsm::Identifier id = circuit->id();

	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Input))
	{
		m_inputs.erase(id);
	}

	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Output))
	{
		m_outputs.erase(id);
	}

	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Wiring))
	{
		m_connectorWires.erase(id);
	}

	// Disconnect and remove.
	m_components.erase(found);

	// As it was naturally erased, so then it's now really erased from circuit.
	return true;
}

bool lcsm::LCSMCircuit::remove(lcsm::Identifier id)
{
	return remove(find(id));
}

bool lcsm::LCSMCircuit::remove(lcsm::label_t name)
{
	return remove(find(name));
}

bool lcsm::LCSMCircuit::remove(const std::string &name)
{
	return remove(name.c_str());
}

const lcsm::LCSMCircuit *lcsm::LCSMCircuit::addCircuit(const lcsm::LCSMCircuit &other)
{
	std::shared_ptr< lcsm::LCSMCircuit > circuit = std::make_shared< lcsm::LCSMCircuit >();
	const lcsm::Identifier circuitId = m_globalId;
	other.copyImpl(circuit.get(), m_globalId);
	m_circuits[circuitId] = circuit;
	m_globalId = circuit->globalId().next();
	return circuit.get();
}

const lcsm::LCSMCircuit *lcsm::LCSMCircuit::findCircuit(lcsm::Identifier circuitId) noexcept
{
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::iterator found = m_circuits.find(circuitId);
	if (found == m_circuits.end())
	{
		return nullptr;
	}
	return found->second.get();
}

const lcsm::LCSMCircuit *lcsm::LCSMCircuit::findCircuit(lcsm::label_t name) noexcept
{
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::const_iterator found = std::find_if(
		m_circuits.begin(),
		m_circuits.end(),
		[name](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &i)
		{ return std::strcmp(i.second->c_name(), name) == 0; });
	if (found == m_circuits.end())
	{
		return nullptr;
	}
	return found->second.get();
}

const lcsm::LCSMCircuit *lcsm::LCSMCircuit::findCircuit(const std::string &name) noexcept
{
	return findCircuit(name.c_str());
}

bool lcsm::LCSMCircuit::removeCircuit(const lcsm::LCSMCircuit *circuit)
{
	// If `circuit` is nullptr, do nothing.
	if (circuit == nullptr)
	{
		return false;
	}

	// Find this circuit, if not found - exit.
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::const_iterator found = std::find_if(
		m_circuits.begin(),
		m_circuits.end(),
		[circuit](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &i)
		{ return i.second.get() == circuit; });
	if (found == m_circuits.end())
	{
		return false;
	}

	// Erase this circuit and as it's done, return `true`.
	m_circuits.erase(found);
	return true;
}

bool lcsm::LCSMCircuit::removeCircuit(lcsm::Identifier id)
{
	return removeCircuit(findCircuit(id));
}

bool lcsm::LCSMCircuit::removeCircuit(lcsm::label_t name)
{
	return removeCircuit(findCircuit(name));
}

bool lcsm::LCSMCircuit::removeCircuit(const std::string &name)
{
	return removeCircuit(findCircuit(name));
}

lcsm::Circuit *lcsm::LCSMCircuit::registerElement(std::shared_ptr< lcsm::Circuit > &&circuit)
{
	const std::size_t numOfWires = circuit->numOfWires();
	std::vector< std::shared_ptr< lcsm::model::Wire > > wires;
	for (std::size_t i = 0; i < numOfWires; i++)
		wires.push_back(std::make_shared< lcsm::model::Wire >());
	circuit->provideWires(wires);
	const lcsm::Identifier prev = m_globalId;
	m_globalId = circuit->identify(prev);
	m_components[prev] = circuit;
	for (std::size_t i = 0; i < numOfWires; i++)
	{
		const lcsm::Identifier id = wires[i]->id();
		m_componentWires[id] = wires[i];
	}
	const lcsm::object_type_t objectType = circuit->objectType();
	const lcsm::Identifier id = circuit->id();
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Input))
		m_inputs[id] = circuit;
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Output))
		m_outputs[id] = circuit;
	return circuit.get();
}

static void
	GetEdges(std::vector< std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > > &edges, const lcsm::model::Wire *wire, bool isFirstComp)
{
	const lcsm::Identifier id1 = wire->id();
	for (const lcsm::support::PointerView< lcsm::Circuit > &child : wire->wires())
	{
		const lcsm::model::Wire *w = static_cast< const lcsm::model::Wire * >(child.get());
		const lcsm::Identifier id2 = child->id();
		const bool isSecondComp = w->connect().hasValue();
		edges.emplace_back(id1, id2, isFirstComp, isSecondComp);
	}
}

void lcsm::LCSMCircuit::copyImpl(lcsm::LCSMCircuit *newCircuit, const Identifier &entryId) const
{
	// Re-entry global Id.
	newCircuit->m_globalId = entryId;

	// Stage 1.1. Re-create all needed components, remember old-to-new-ids (as entryId might be not as global id).
	std::unordered_map< lcsm::Identifier, lcsm::Identifier > oldToNewId;
	std::vector< std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > > edges;
	std::vector< std::pair< lcsm::Identifier, lcsm::Identifier > > tunnels;
	for (const auto &component : m_components)
	{
		const std::shared_ptr< lcsm::Circuit > &oldCircuit = component.second;
		switch (oldCircuit->circuitType())
		{
		case lcsm::CircuitType::Wire:
		{
			throw std::logic_error("Wire in components? The fuck you have done?!");
		}
		case lcsm::CircuitType::Tunnel:
		{
			const lcsm::model::Tunnel *oldTunnel = static_cast< const lcsm::model::Tunnel * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldTunnel->wire();
			const std::vector< lcsm::Circuit * > &oldTunnels = oldTunnel->tunnels();
			const lcsm::label_t name = oldTunnel->c_name();
			const lcsm::model::Tunnel *newTunnel = newCircuit->createTunnel(name);
			const lcsm::Identifier id = newTunnel->id();
			oldToNewId[oldTunnel->id()] = id;
			oldToNewId[oldWire->id()] = newTunnel->wire()->id();
			GetEdges(edges, oldWire, true);
			for (lcsm::Circuit *tunnel : oldTunnels)
			{
				const lcsm::Identifier tunnelId = tunnel->id();
				tunnels.emplace_back(id, tunnelId);
			}
			break;
		}
		case lcsm::CircuitType::Pin:
		{
			const lcsm::model::Pin *oldPin = static_cast< const lcsm::model::Pin * >(oldCircuit.get());
			const lcsm::model::Wire *oldInternal = oldPin->internal();
			const lcsm::model::Wire *oldExternal = oldPin->external();
			const lcsm::label_t name = oldPin->c_name();
			const bool output = oldPin->output();
			const lcsm::Width width = oldPin->width();
			const lcsm::model::Pin *newPin = newCircuit->createPin(output, name, width);
			oldToNewId[oldPin->id()] = newPin->id();
			oldToNewId[oldInternal->id()] = newPin->internal()->id();
			oldToNewId[oldExternal->id()] = newPin->external()->id();
			GetEdges(edges, oldInternal, true);
			GetEdges(edges, oldExternal, true);
			break;
		}
		case lcsm::CircuitType::Constant:
		{
			const lcsm::model::Constant *oldConstant = static_cast< const lcsm::model::Constant * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldConstant->wire();
			const lcsm::label_t name = oldConstant->c_name();
			const lcsm::Width width = oldConstant->width();
			const lcsm::value_t value = oldConstant->value();
			const lcsm::model::Constant *newConstant = newCircuit->createConstant(name, width, value);
			oldToNewId[oldConstant->id()] = newConstant->id();
			oldToNewId[oldWire->id()] = newConstant->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Power:
		{
			const lcsm::model::Power *oldPower = static_cast< const lcsm::model::Power * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldPower->wire();
			const lcsm::label_t name = oldWire->c_name();
			const lcsm::Width width = oldPower->width();
			const lcsm::model::Power *power = newCircuit->createPower(name, width);
			oldToNewId[oldPower->id()] = power->id();
			oldToNewId[oldWire->id()] = power->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Ground:
		{
			const lcsm::model::Ground *oldGround = static_cast< const lcsm::model::Ground * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldGround->wire();
			const lcsm::label_t name = oldGround->c_name();
			const lcsm::Width width = oldGround->width();
			const lcsm::model::Ground *newGround = newCircuit->createGround(name, width);
			oldToNewId[oldGround->id()] = newGround->id();
			oldToNewId[oldWire->id()] = newGround->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Clock:
		{
			const lcsm::model::Clock *oldClock = static_cast< const lcsm::model::Clock * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldClock->wire();
			const lcsm::label_t name = oldClock->c_name();
			const unsigned highDuration = oldClock->highDuration();
			const unsigned lowDuration = oldClock->lowDuration();
			const unsigned phaseOffset = oldClock->phaseOffset();
			const lcsm::model::Clock *newClock = newCircuit->createClock(name, highDuration, lowDuration, phaseOffset);
			oldToNewId[oldClock->id()] = newClock->id();
			oldToNewId[oldWire->id()] = newClock->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Transistor:
		{
			const lcsm::model::Transistor *oldTransistor = static_cast< const lcsm::model::Transistor * >(oldCircuit.get());
			const lcsm::model::Wire *oldWireBase = oldTransistor->wireBase();
			const lcsm::model::Wire *oldWireSrcA = oldTransistor->wireSrcA();
			const lcsm::model::Wire *oldWireSrcB = oldTransistor->wireSrcB();
			const lcsm::label_t name = oldTransistor->c_name();
			const lcsm::model::Transistor::Type type = oldTransistor->type();
			const lcsm::model::Transistor *newTransistor = newCircuit->createTransistor(name, type);
			oldToNewId[oldTransistor->id()] = newTransistor->id();
			oldToNewId[oldWireBase->id()] = newTransistor->wireBase()->id();
			oldToNewId[oldWireSrcA->id()] = newTransistor->wireSrcA()->id();
			oldToNewId[oldWireSrcB->id()] = newTransistor->wireSrcB()->id();
			GetEdges(edges, oldWireBase, true);
			GetEdges(edges, oldWireSrcA, true);
			GetEdges(edges, oldWireSrcB, true);
			break;
		}
		case lcsm::CircuitType::TransmissionGate:
		{
			const lcsm::model::TransmissionGate *oldTransmissionGate =
				static_cast< const lcsm::model::TransmissionGate * >(oldCircuit.get());
			const lcsm::model::Wire *oldWireBase = oldTransmissionGate->wireBase();
			const lcsm::model::Wire *oldWireSrcA = oldTransmissionGate->wireSrcA();
			const lcsm::model::Wire *oldWireSrcB = oldTransmissionGate->wireSrcB();
			const lcsm::model::Wire *oldWireSrcC = oldTransmissionGate->wireSrcB();
			const lcsm::label_t name = oldTransmissionGate->c_name();
			const lcsm::model::TransmissionGate *newTransmissionGate = newCircuit->createTransmissionGate(name);
			oldToNewId[oldTransmissionGate->id()] = newTransmissionGate->id();
			oldToNewId[oldWireBase->id()] = newTransmissionGate->wireBase()->id();
			oldToNewId[oldWireSrcA->id()] = newTransmissionGate->wireSrcA()->id();
			oldToNewId[oldWireSrcB->id()] = newTransmissionGate->wireSrcB()->id();
			oldToNewId[oldWireSrcC->id()] = newTransmissionGate->wireSrcB()->id();
			GetEdges(edges, oldWireBase, true);
			GetEdges(edges, oldWireSrcA, true);
			GetEdges(edges, oldWireSrcB, true);
			GetEdges(edges, oldWireSrcC, true);
			break;
		}
		case lcsm::CircuitType::Button:
		{
			const lcsm::model::Button *oldButton = static_cast< const lcsm::model::Button * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldButton->wire();
			const lcsm::label_t name = oldButton->c_name();
			const bool activeOnPress = oldButton->activeOnPress();
			const lcsm::model::Button *newButton = newCircuit->createButton(name, activeOnPress);
			oldToNewId[oldButton->id()] = newButton->id();
			oldToNewId[oldWire->id()] = newButton->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Digit:
		{
			const lcsm::model::Digit *oldDigit = static_cast< const lcsm::model::Digit * >(oldCircuit.get());
			const lcsm::model::Wire *oldWireData = oldDigit->wireData();
			const lcsm::model::Wire *oldWireDecimalPoint = oldDigit->wireDecimalPoint();
			const lcsm::label_t name = oldDigit->c_name();
			const bool hasDecimalPoint = oldDigit->hasDecimalPoint();
			const lcsm::model::Digit *newDigit = newCircuit->createDigit(name, hasDecimalPoint);
			oldToNewId[oldDigit->id()] = newDigit->id();
			oldToNewId[oldWireData->id()] = newDigit->wireData()->id();
			oldToNewId[oldWireDecimalPoint->id()] = newDigit->wireDecimalPoint()->id();
			GetEdges(edges, oldWireData, true);
			GetEdges(edges, oldWireDecimalPoint, true);
			break;
		}
		case lcsm::CircuitType::Probe:
		{
			const lcsm::model::Probe *oldProbe = static_cast< const lcsm::model::Probe * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldProbe->wire();
			const lcsm::label_t name = oldProbe->c_name();
			const lcsm::model::Probe *newProbe = newCircuit->createProbe(name);
			oldToNewId[oldProbe->id()] = newProbe->id();
			oldToNewId[oldWire->id()] = newProbe->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Splitter:
		{
			const lcsm::model::Splitter *oldSplitter = static_cast< const lcsm::model::Splitter * >(oldCircuit.get());
			const lcsm::model::Wire *oldWireIn = oldSplitter->wireIn();
			const lcsm::label_t name = oldSplitter->c_name();
			const lcsm::Width widthIn = oldSplitter->widthIn();
			const lcsm::width_t widthOut = oldSplitter->widthOut();
			const lcsm::model::Splitter *newSplitter = newCircuit->createSplitter(name, widthIn, widthOut);
			oldToNewId[oldSplitter->id()] = newSplitter->id();
			oldToNewId[oldWireIn->id()] = newSplitter->wireIn()->id();
			GetEdges(edges, oldSplitter->wireIn(), true);
			for (lcsm::portid_t i = 0; i < static_cast< lcsm::portid_t >(widthOut); i++)
			{
				const lcsm::model::Wire *oldWireOut = oldSplitter->wireOut(i);
				oldToNewId[oldWireOut->id()] = newSplitter->wireOut(i)->id();
				GetEdges(edges, oldSplitter->wireOut(i), true);
			}
			break;
		}
		case lcsm::CircuitType::LastCircuitType:
			break;
		}
	}

	// Stage 1.2. Re-create all wires as 1.1.
	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &connector : m_connectorWires)
	{
		const lcsm::model::Wire *oldWire = static_cast< const lcsm::model::Wire * >(connector.second.get());
		std::shared_ptr< lcsm::Circuit > wire = newCircuit->createConnectorWire(oldWire->c_name());
		oldToNewId[oldWire->id()] = wire->id();
		GetEdges(edges, oldWire, false);
	}

	// Stage 2. Copy in within circuits.
	for (const auto &circuit : m_circuits)
	{
		newCircuit->addCircuit(*circuit.second);
	}

	// Stage 3. Make connections.
	for (const std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > &edge : edges)
	{
		const lcsm::Identifier wireId1 = oldToNewId[std::get< 0 >(edge)];
		const lcsm::Identifier wireId2 = oldToNewId[std::get< 1 >(edge)];
		const bool isFirstComp = std::get< 2 >(edge);
		const bool isSecondComp = std::get< 3 >(edge);
		lcsm::model::Wire *wire1 = nullptr;
		lcsm::model::Wire *wire2 = nullptr;
		if (isFirstComp)
			wire1 = static_cast< lcsm::model::Wire * >(newCircuit->m_componentWires[wireId1].get());
		else
			wire1 = static_cast< lcsm::model::Wire * >(newCircuit->m_connectorWires[wireId1].get());
		if (isSecondComp)
			wire2 = static_cast< lcsm::model::Wire * >(newCircuit->m_componentWires[wireId2].get());
		else
			wire2 = static_cast< lcsm::model::Wire * >(newCircuit->m_connectorWires[wireId2].get());
		wire1->connectToWire(wire2);
	}

	// Stage 4. Make tunnel's connections.
	for (const std::pair< lcsm::Identifier, lcsm::Identifier > &tunnel : tunnels)
	{
		const lcsm::Identifier id1 = oldToNewId[tunnel.first];
		const lcsm::Identifier id2 = oldToNewId[tunnel.second];
		lcsm::model::Tunnel *tunnel1 = static_cast< lcsm::model::Tunnel * >(newCircuit->m_components[id1].get());
		lcsm::model::Tunnel *tunnel2 = static_cast< lcsm::model::Tunnel * >(newCircuit->m_components[id2].get());
		tunnel1->connectTunnel(tunnel2);
		tunnel2->connectTunnel(tunnel1);
	}
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::copyImpl(const lcsm::Identifier &entryId) const
{
	lcsm::LCSMCircuit newCircuit(c_name());
	copyImpl(std::addressof(newCircuit), entryId);
	return newCircuit;
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createHeadlessWire(lcsm::label_t name)
{
	return std::make_shared< lcsm::model::Wire >(name);
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createIdentifiedWire(lcsm::label_t name)
{
	std::shared_ptr< lcsm::model::Wire > wire = createHeadlessWire(name);
	m_globalId = wire->identify(m_globalId);
	return wire;
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createConnectorWire(lcsm::label_t name)
{
	std::shared_ptr< lcsm::model::Wire > wire = createIdentifiedWire(name);
	m_connectorWires[wire->id()] = wire;
	return wire;
}
