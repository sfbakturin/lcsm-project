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
#include <map>
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

lcsm::Identifier lcsm::LCSMCircuit::globalId() const noexcept
{
	return m_globalId;
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

static std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator
	FindCircuit(const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &m, lcsm::Circuit *c)
{
	return std::find_if(
		m.begin(),
		m.end(),
		[c](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i) { return i.second.get() == c; });
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::portid_t port1, lcsm::Circuit *circuit2, lcsm::portid_t port2)
{
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
	std::shared_ptr< lcsm::model::Wire > wire = createConnectorWire();
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
	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator found = m_components.begin();
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
		m_connectorWires.erase(id);
	// Disconnect and remove.
	m_components.erase(found);
}

lcsm::Circuit *lcsm::LCSMCircuit::find(lcsm::Identifier id) noexcept
{
	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator found = m_components.find(id);
	if (found == m_components.end())
		return nullptr;
	return found->second.get();
}

const lcsm::LCSMCircuit *lcsm::LCSMCircuit::addCircuit(const lcsm::LCSMCircuit &other)
{
	std::shared_ptr< lcsm::LCSMCircuit > circuit = std::make_shared< lcsm::LCSMCircuit >();
	const lcsm::Identifier circuitId = m_globalId;
	other.copyImpl(circuit.get(), m_globalId);
	m_circuits[circuitId] = circuit;
	return circuit.get();
}

const lcsm::LCSMCircuit *lcsm::LCSMCircuit::findCircuit(lcsm::Identifier circuitId) noexcept
{
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::iterator found = m_circuits.find(circuitId);
	if (found != m_circuits.end())
	{
		const std::shared_ptr< lcsm::LCSMCircuit > &circuit = found->second;
		return circuit.get();
	}
	return nullptr;
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
	for (const auto &component : m_components)
	{
		const std::shared_ptr< lcsm::Circuit > &oldCircuit = component.second;
		switch (oldCircuit->circuitType())
		{
		case lcsm::CircuitType::Wire:
			throw std::logic_error("Wire in components? The fuck you have done?!");
		case lcsm::CircuitType::Tunnel:
			throw std::logic_error("Not implemented.");
		case lcsm::CircuitType::Pin:
		{
			const lcsm::model::Pin *oldPin = static_cast< const lcsm::model::Pin * >(oldCircuit.get());
			const lcsm::model::Wire *oldInternal = oldPin->internal();
			const lcsm::model::Wire *oldExternal = oldPin->external();
			const bool output = oldPin->output();
			const lcsm::Width width = oldPin->width();
			const lcsm::model::Pin *newPin = newCircuit->createPin(output, width);
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
			const lcsm::Width width = oldConstant->width();
			const lcsm::value_t value = oldConstant->value();
			const lcsm::model::Constant *newConstant = newCircuit->createConstant(width, value);
			oldToNewId[oldConstant->id()] = newConstant->id();
			oldToNewId[oldWire->id()] = newConstant->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Power:
		{
			const lcsm::model::Power *oldPower = static_cast< const lcsm::model::Power * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldPower->wire();
			const lcsm::Width width = oldPower->width();
			const lcsm::model::Power *power = newCircuit->createPower(width);
			oldToNewId[oldPower->id()] = power->id();
			oldToNewId[oldWire->id()] = power->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Ground:
		{
			const lcsm::model::Ground *oldGround = static_cast< const lcsm::model::Ground * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldGround->wire();
			const lcsm::Width width = oldGround->width();
			const lcsm::model::Ground *newGround = newCircuit->createGround(width);
			oldToNewId[oldGround->id()] = newGround->id();
			oldToNewId[oldWire->id()] = newGround->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Clock:
		{
			const lcsm::model::Clock *oldClock = static_cast< const lcsm::model::Clock * >(oldCircuit.get());
			const lcsm::model::Wire *oldWire = oldClock->wire();
			const unsigned highDuration = oldClock->highDuration();
			const unsigned lowDuration = oldClock->lowDuration();
			const unsigned phaseOffset = oldClock->phaseOffset();
			const lcsm::model::Clock *newClock = newCircuit->createClock(highDuration, lowDuration, phaseOffset);
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
			const lcsm::model::Transistor::Type type = oldTransistor->type();
			const lcsm::model::Transistor *newTransistor = newCircuit->createTransistor(type);
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
			const lcsm::model::TransmissionGate *newTransmissionGate = newCircuit->createTransmissionGate();
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
			const bool activeOnPress = oldButton->activeOnPress();
			const lcsm::model::Button *newButton = newCircuit->createButton(activeOnPress);
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
			const bool hasDecimalPoint = oldDigit->hasDecimalPoint();
			const lcsm::model::Digit *newDigit = newCircuit->createDigit(hasDecimalPoint);
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
			const lcsm::model::Probe *newProbe = newCircuit->createProbe();
			oldToNewId[oldProbe->id()] = newProbe->id();
			oldToNewId[oldWire->id()] = newProbe->wire()->id();
			GetEdges(edges, oldWire, true);
			break;
		}
		case lcsm::CircuitType::Splitter:
		{
			const lcsm::model::Splitter *oldSplitter = static_cast< const lcsm::model::Splitter * >(oldCircuit.get());
			const lcsm::model::Wire *oldWireIn = oldSplitter->wireIn();
			const lcsm::Width widthIn = oldSplitter->widthIn();
			const lcsm::width_t widthOut = oldSplitter->widthOut();
			const lcsm::model::Splitter *newSplitter = newCircuit->createSplitter(widthIn, widthOut);
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
		}
	}

	// Stage 1.2. Re-create all wires as 1.1.
	for (const auto &connector : m_connectorWires)
	{
		const lcsm::model::Wire *oldWire = static_cast< const lcsm::model::Wire * >(connector.second.get());
		std::shared_ptr< lcsm::Circuit > wire = newCircuit->createConnectorWire();
		oldToNewId[oldWire->id()] = wire->id();
		GetEdges(edges, oldWire, false);
	}

	// Stage 2. Copy in within circuits.
	for (const auto &circuit : m_circuits)
		newCircuit->addCircuit(*circuit.second);

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
		wire2->connectToWire(wire1);
	}
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::copyImpl(const lcsm::Identifier &entryId) const
{
	lcsm::LCSMCircuit newCircuit;
	copyImpl(std::addressof(newCircuit), entryId);
	return newCircuit;
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createHeadlessWire()
{
	return std::make_shared< lcsm::model::Wire >();
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createIdentifiedWire()
{
	std::shared_ptr< lcsm::model::Wire > wire = createHeadlessWire();
	m_globalId = wire->identify(m_globalId);
	return wire;
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createConnectorWire()
{
	std::shared_ptr< lcsm::model::Wire > wire = createIdentifiedWire();
	m_connectorWires[wire->id()] = wire;
	return wire;
}
