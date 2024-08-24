#include <initializer_list>
#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Component.h>
#include <sim/Component/WiringComponent.h>
#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/SimCircuit.h>
#include <sim/SimContext.h>
#include <sim/SimEngine.h>
#include <sim/SimState.h>
#include <sim/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <deque>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

sim::SimEngine::SimEngine() : m_circuits(0) {}

void sim::SimEngine::addCircuit(sim::SimCircuit &circuit)
{
	if (m_circuits != 0)
		throw std::logic_error("");

	m_circuits++;

	std::deque< sim::support::PointerView< const sim::Component > > bfsVisit;

	for (auto it = circuit.Pins().begin(); it != circuit.Pins().end(); it++)
	{
		const sim::Pin *pin = it->second->AsCircuit()->AsPin();

		if (pin->IsOutput())
			continue;

		// Initialize future nodes for calculation graph.
		sim::CGPin *pinGraph = registeredPinInput(pin->ID());
		sim::CGNode *pinNode = registeredNode(pin->ID(), pinGraph);
		m_inputs.addRoot({ pinNode });

		bfsVisit.emplace_back(pin);
	}

	buildCircuit(bfsVisit);
}

std::vector< sim::Value > sim::SimEngine::invokeFull(std::initializer_list< sim::Value >)
{
	return {};
}

sim::CGPinInput *sim::SimEngine::registerPinInput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinInput found same ID.");

	std::shared_ptr< CGObject > I = std::make_shared< sim::CGPinInput >();
	m_objects[ID] = I;

	return static_cast< sim::CGPinInput * >(I.get());
}

sim::CGPinOutput *sim::SimEngine::registerPinOutput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinOutput found same ID.");

	std::shared_ptr< CGObject > O = std::make_shared< sim::CGPinOutput >();
	m_objects[ID] = O;

	return static_cast< sim::CGPinOutput * >(O.get());
}

sim::CGWire *sim::SimEngine::registerWire(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Wire found same ID.");

	std::shared_ptr< CGObject > W = std::make_shared< sim::CGWire >();
	m_objects[ID] = W;

	return static_cast< sim::CGWire * >(W.get());
}

sim::CGPinInput *sim::SimEngine::registeredPinInput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
	{
		sim::CGObject *I = found->second.get();
		if (I && I->isPinInput())
			return I->asPinInput();
		else
			throw std::logic_error("RegisteredPinInput");
	}

	return registerPinInput(ID);
}

sim::CGPinOutput *sim::SimEngine::registeredPinOutput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
	{
		CGObject *I = found->second.get();
		if (I && I->isPinOutput())
			return I->asPinOutput();
		else
			throw std::logic_error("RegisteredPinOutput");
	}

	return registerPinOutput(ID);
}

sim::CGWire *sim::SimEngine::registeredWire(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
	{
		CGObject *I = found->second.get();
		if (I && I->isWire())
			return I->asWire();
		else
			throw std::logic_error("RegisteredPinOutput");
	}

	return registerWire(ID);
}

sim::CGNode *sim::SimEngine::registeredNode(unsigned ID, CGObject *object)
{
	std::unordered_map< unsigned, sim::CGNode >::iterator found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second.object().Ptr() == object)
			return std::addressof(found->second);
		else
			throw std::logic_error("Node registered, objects not same");
	}

	m_nodes[ID] = object;

	return std::addressof(m_nodes[ID]);
}

void sim::SimEngine::buildCircuitIOComp(
	std::unordered_set< unsigned > &,
	std::deque< sim::support::PointerView< const sim::Component > > &,
	const sim::IOComponent *)
{
	throw std::logic_error("Not implemented");
}

void sim::SimEngine::buildCircuitWiringComp(
	std::unordered_set< unsigned > &visited,
	std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
	const sim::WiringComponent *wiringComp)
{
	switch (wiringComp->wiringComponentType())
	{
	case WIRING_COMP_WIRE:
	{
		const sim::Wire *wire = wiringComp->AsWire();
		sim::CGWire *wireGraph = registeredWire(wire->ID());

		// Wire's tree node.
		sim::CGNode *wireNode = registeredNode(wire->ID(), wireGraph);

		// Make connections from Wire object to his adjacent wires as
		// tree's edges.
		for (const sim::wire_t &adjacentWire : wire->Wires())
		{
			switch (adjacentWire->wiringComponentType())
			{
			case WIRING_COMP_WIRE:
			{
				// Wire's adjacent wire tree node.
				sim::CGWire *adjacentWireGraph = registeredWire(adjacentWire->ID());
				sim::CGNode *adjacentWireNode =
					registeredNode(adjacentWire->ID(), adjacentWireGraph);

				// Broadcast value from Wire to adjacent Wire object as
				// tree's edge.
				std::shared_ptr< sim::Instruction > wvWireToWire =
					sim::CreateBroadcastValue(wireGraph, adjacentWireGraph);
				sim::CGEdge wireToWireEdge = { { wireGraph }, { adjacentWireGraph }, std::move(wvWireToWire) };
				wireNode->addInstruction(std::move(wireToWireEdge), adjacentWireNode);

				break;
			}
			case WIRING_COMP_TUNNEL:
				throw std::logic_error("Not implemented.");
			}
			// Add to future BFS visiting.
			bfsVisit.emplace_back(adjacentWire.Ptr());
		}

		// This wire might be som object's wire, so we should make connection as
		// tree's edge.
		for (const sim::component_t &comp : wire->Connections())
		{
			switch (comp->componentType())
			{
			case COMP_IO:
				throw std::logic_error("Not implemented");
			case COMP_WIRING:
				throw std::logic_error("Impossible");
			case COMP_CIRCUIT:
			{
				const sim::CircuitComponent *circComp = comp->AsCircuit();
				switch (circComp->circuitComponentType())
				{
				case CIRCUIT_COMP_PIN:
				{
					const sim::Pin *pin = circComp->AsPin();
					sim::CGPin *pinGraph = nullptr;

					if (pin->IsOutput())
						pinGraph = registeredPinOutput(pin->ID());
					else
						pinGraph = registeredPinInput(pin->ID());

					// Make connection between Pin's Wire to Pin object as tree
					// node.
					sim::CGNode *pinNode = registeredNode(pin->ID(), pinGraph);

					std::shared_ptr< sim::Instruction > wvWireToPin =
						sim::CreateWriteValue(wireGraph, pinGraph);
					sim::CGEdge wireToPinEdge = { { wireGraph }, { pinGraph }, std::move(wvWireToPin) };
					wireNode->addInstruction(std::move(wireToPinEdge), pinNode);

					break;
				}
				case CIRCUIT_COMP_CONSTANT:
					throw std::logic_error("Not implemented");
				case CIRCUIT_COMP_POWER:
					throw std::logic_error("Not implemented");
				case CIRCUIT_COMP_GROUND:
					throw std::logic_error("Not implemented");
				case CIRCUIT_COMP_SPLITTER:
					throw std::logic_error("Not implemented");
				case CIRCUIT_COMP_CLOCK:
					throw std::logic_error("Not implemented");
				case CIRCUIT_COMP_TRANSISTOR:
					throw std::logic_error("Not implemented");
				case CIRCUIT_COMP_TRANSMISSION_GATE:
					throw std::logic_error("Not implemented");
				}
			}
			}
			// Add to future BFS visiting.
			bfsVisit.emplace_back(comp.Ptr());
		}

		// Add as visited objects.
		visited.insert(wire->ID());

		break;
	}
	case WIRING_COMP_TUNNEL:
		throw std::logic_error("Not implemented.");
	}
}

void sim::SimEngine::buildCircuitCircuitComp(
	std::unordered_set< unsigned > &visited,
	std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
	const sim::CircuitComponent *circuitComp)
{
	switch (circuitComp->circuitComponentType())
	{
	case sim::CircuitComponentType::CIRCUIT_COMP_PIN:
	{
		const sim::Pin *pin = circuitComp->AsPin();
		const sim::Wire &pinWire = pin->GetWire();

		sim::CGPin *pinGraph = nullptr;

		if (pin->IsOutput())
			pinGraph = registeredPinOutput(pin->ID());
		else
			pinGraph = registeredPinInput(pin->ID());

		sim::CGWire *pinWireGraph = registeredWire(pinWire.ID());

		// Pin's tree node.
		sim::CGNode *pinNode = registeredNode(pin->ID(), pinGraph);

		// Wire's tree node as Pin's child.
		sim::CGNode *wireNode = registeredNode(pinWire.ID(), pinWireGraph);

		// Write value from Pin to Pin's Wire object as tree's edge.
		std::shared_ptr< sim::Instruction > wvPinToWire =
			sim::CreateWriteValue(pinGraph, pinWireGraph);
		sim::CGEdge pinToWireEdge = { { pinGraph }, { pinWireGraph }, std::move(wvPinToWire) };
		pinNode->addInstruction(std::move(pinToWireEdge), wireNode);

		// Write value from Pin's Wire object to Pin as tree's edge.
		std::shared_ptr< sim::Instruction > wvWireToPin =
			sim::CreateWriteValue(pinWireGraph, pinGraph);
		sim::CGEdge wireToPinEdge = { { pinWireGraph }, { pinGraph }, std::move(wvWireToPin) };
		wireNode->addInstruction(std::move(wireToPinEdge), pinNode);

		// Make connections from Pin's Wire object to his adjacent wires as
		// tree's edges.
		for (const sim::wire_t &adjacentWire : pinWire.Wires())
		{
			switch (adjacentWire->wiringComponentType())
			{
			case WIRING_COMP_WIRE:
			{
				// Wire's adjacent wire tree node.
				sim::CGWire *adjacentWireGraph = registeredWire(adjacentWire->ID());
				sim::CGNode *adjacentWireNode =
					registeredNode(adjacentWire->ID(), adjacentWireGraph);

				// Broadcast value from Pin's Wire to adjacent Wire object as
				// tree's edge.
				std::shared_ptr< sim::Instruction > wvWireToWire =
					sim::CreateBroadcastValue(pinWireGraph, adjacentWireGraph);
				sim::CGEdge wireToWireEdge = { { pinWireGraph }, { adjacentWireGraph }, std::move(wvWireToWire) };
				wireNode->addInstruction(std::move(wireToWireEdge), adjacentWireNode);

				break;
			}
			case WIRING_COMP_TUNNEL:
				throw std::logic_error("Not implemented.");
			}
			// Add to future BFS visiting.
			bfsVisit.emplace_back(adjacentWire.Ptr());
		}

		// Add as visited objects.
		visited.insert(pin->ID());
		visited.insert(pinWire.ID());

		break;
	}
	case sim::CircuitComponentType::CIRCUIT_COMP_CONSTANT:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_POWER:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_GROUND:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_SPLITTER:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_CLOCK:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_TRANSISTOR:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_TRANSMISSION_GATE:
		throw std::logic_error("Not implemented");
	}
}

void sim::SimEngine::buildCircuit(std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit)
{
	std::unordered_set< unsigned > visited;

	while (!bfsVisit.empty())
	{
		std::size_t levelSize = bfsVisit.size();

		while (levelSize != 0)
		{
			levelSize--;

			sim::support::PointerView< const sim::Component > comp = bfsVisit.front();
			bfsVisit.pop_front();

			const std::unordered_set< unsigned >::const_iterator found =
				visited.find(comp->ID());
			if (found != visited.cend())
				continue;

			switch (comp->componentType())
			{
			case COMP_IO:
				buildCircuitIOComp(visited, bfsVisit, comp->AsIO());
				break;
			case COMP_WIRING:
				buildCircuitWiringComp(visited, bfsVisit, comp->AsWiring());
				break;
			case COMP_CIRCUIT:
				buildCircuitCircuitComp(visited, bfsVisit, comp->AsCircuit());
				break;
			}
		}
	}
}
