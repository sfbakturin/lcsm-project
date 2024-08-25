#include "sim/Model/Circuit/Constant.h"
#include "sim/Model/Circuit/Ground.h"
#include "sim/Model/Circuit/Power.h"
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
		const sim::Pin *pin = it->second->AsCircuit()->asPin();

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

sim::CGWire *sim::SimEngine::registerWire(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Wire found same ID.");

	std::shared_ptr< sim::CGObject > W = std::make_shared< sim::CGWire >();
	m_objects[ID] = W;

	return static_cast< sim::CGWire * >(W.get());
}

sim::CGPinInput *sim::SimEngine::registerPinInput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinInput found same ID.");

	std::shared_ptr< sim::CGObject > I = std::make_shared< sim::CGPinInput >();
	m_objects[ID] = I;

	return static_cast< sim::CGPinInput * >(I.get());
}

sim::CGPinOutput *sim::SimEngine::registerPinOutput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinOutput found same ID.");

	std::shared_ptr< sim::CGObject > O = std::make_shared< sim::CGPinOutput >();
	m_objects[ID] = O;

	return static_cast< sim::CGPinOutput * >(O.get());
}

sim::CGConstant *sim::SimEngine::registerConstant(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Constant found same ID.");

	std::shared_ptr< sim::CGObject > C = std::make_shared< sim::CGConstant >();
	m_objects[ID] = C;

	return static_cast< sim::CGConstant * >(C.get());
}

sim::CGPower *sim::SimEngine::registerPower(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Power found same ID.");

	std::shared_ptr< sim::CGObject > P = std::make_shared< sim::CGPower >();
	m_objects[ID] = P;

	return static_cast< sim::CGPower * >(P.get());
}

sim::CGGround *sim::SimEngine::registerGround(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Ground found same ID.");

	std::shared_ptr< sim::CGObject > G = std::make_shared< sim::CGGround >();
	m_objects[ID] = G;

	return static_cast< sim::CGGround * >(G.get());
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

sim::CGConstant *sim::SimEngine::registeredConstant(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
	{
		sim::CGObject *I = found->second.get();
		if (I && I->isConstant())
			return I->asConstant();
		else
			throw std::logic_error("RegisteredConstant");
	}

	return registerConstant(ID);
}

sim::CGPower *sim::SimEngine::registeredPower(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
	{
		sim::CGObject *I = found->second.get();
		if (I && I->isPower())
			return I->asPower();
		else
			throw std::logic_error("RegisteredPower");
	}

	return registerPower(ID);
}

sim::CGGround *sim::SimEngine::registeredGround(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< sim::CGObject > >::const_iterator found =
		m_objects.find(ID);

	if (found != m_objects.cend())
	{
		sim::CGObject *I = found->second.get();
		if (I && I->isGround())
			return I->asGround();
		else
			throw std::logic_error("RegisteredPower");
	}

	return registerGround(ID);
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
				sim::InstructionShared wvWireToWire =
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
					const sim::Pin *pin = circComp->asPin();
					sim::CGPin *pinGraph = nullptr;

					if (pin->IsOutput())
						pinGraph = registeredPinOutput(pin->ID());
					else
						pinGraph = registeredPinInput(pin->ID());

					// Make connection between Pin's Wire to Pin object as tree
					// node.
					sim::CGNode *pinNode = registeredNode(pin->ID(), pinGraph);

					sim::InstructionShared wvWireToPin =
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
	sim::CGNode *wireNode = nullptr;
	sim::CGWire *outputWireGraph = nullptr;
	const sim::Wire *outputWire = nullptr;

	switch (circuitComp->circuitComponentType())
	{
	case sim::CircuitComponentType::CIRCUIT_COMP_PIN:
	{
		const sim::Pin *pin = circuitComp->asPin();
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
		sim::CGNode *pinWireNode = registeredNode(pinWire.ID(), pinWireGraph);

		// Write value from Pin to Pin's Wire object as tree's edge.
		sim::InstructionShared wvPinToWire = sim::CreateWriteValue(pinGraph, pinWireGraph);
		sim::CGEdge pinToWireEdge = { { pinGraph }, { pinWireGraph }, std::move(wvPinToWire) };
		pinNode->addInstruction(std::move(pinToWireEdge), pinWireNode);

		// Write value from Pin's Wire object to Pin as tree's edge.
		sim::InstructionShared wvWireToPin = sim::CreateWriteValue(pinWireGraph, pinGraph);
		sim::CGEdge wireToPinEdge = { { pinWireGraph }, { pinGraph }, std::move(wvWireToPin) };
		pinWireNode->addInstruction(std::move(wireToPinEdge), pinNode);

		// Set output's wire to make connections from Pin's Wire object.
		wireNode = pinWireNode;
		outputWireGraph = pinWireGraph;
		outputWire = std::addressof(pinWire);

		// Add as visited objects.
		visited.insert(pin->ID());
		visited.insert(pinWire.ID());

		break;
	}
	case sim::CircuitComponentType::CIRCUIT_COMP_CONSTANT:
	{
		const sim::Constant *constant = circuitComp->asConstant();
		const sim::Wire &constantWire = constant->wire();

		sim::CGConstant *constantGraph = registeredConstant(constant->ID());
		sim::CGWire *constantWireGraph = registeredWire(constantWire.ID());

		constantGraph->emplaceValue(constant->GetWidth(), constant->GetValue());

		// Constant's tree node.
		sim::CGNode *constantNode = registeredNode(constant->ID(), constantGraph);

		// Wire's tree node as Constant's child.
		sim::CGNode *constantWireNode = registeredNode(constantWire.ID(), constantWireGraph);

		// Write value from Constant to Constant's Wire object as tree's edge.
		sim::InstructionShared wvConstantToWire =
			sim::CreateWriteValue(constantGraph, constantWireGraph);
		sim::CGEdge constantToWireEdge = { { constantGraph }, { constantWireGraph }, std::move(wvConstantToWire) };
		constantNode->addInstruction(std::move(constantToWireEdge), constantWireNode);

		// Write value from Constant's Wire object to Constant as tree's edge.
		sim::InstructionShared wvWireToConstant =
			sim::CreateWriteValue(constantWireGraph, constantGraph);
		sim::CGEdge wireToConstantEdge = { { constantWireGraph }, { constantGraph }, std::move(wvWireToConstant) };
		constantWireNode->addInstruction(std::move(wireToConstantEdge), constantNode);

		// Set output's wire to make connections from Constant's Wire object.
		wireNode = constantWireNode;
		outputWireGraph = constantWireGraph;
		outputWire = std::addressof(constantWire);

		// Add as visited objects.
		visited.insert(constant->ID());
		visited.insert(constantWire.ID());

		break;
	}
	case sim::CircuitComponentType::CIRCUIT_COMP_POWER:
	{
		const sim::Power *power = circuitComp->asPower();
		const sim::Wire &powerWire = power->wire();

		sim::CGPower *powerGraph = registeredPower(power->ID());
		sim::CGWire *powerWireGraph = registeredWire(powerWire.ID());

		powerGraph->setWidth(power->GetWidth());

		// Power's tree node.
		sim::CGNode *powerNode = registeredNode(power->ID(), powerGraph);

		// Wire's value tree node as Power's child.
		sim::CGNode *powerWireNode = registeredNode(powerWire.ID(), powerWireGraph);

		// Write value from Power to Power's Wire object as tree's edge.
		sim::InstructionShared wvPowerToWire = sim::CreateWriteValue(powerGraph, powerWireGraph);
		sim::CGEdge powerToWireEdge = { { powerGraph }, { powerWireGraph }, std::move(wvPowerToWire) };
		powerNode->addInstruction(std::move(powerToWireEdge), powerWireNode);

		// Write value from Power's Wire object to Power as tree's edge.
		sim::InstructionShared wvWireToPower =
			sim::CreateWriteValue(powerWireGraph, powerGraph);
		sim::CGEdge wireToPowerEdge = { { powerWireGraph }, { powerGraph }, std::move(wvWireToPower) };
		powerWireNode->addInstruction(std::move(wireToPowerEdge), powerNode);

		// Set output's wire to make connections from Power's Wire object.
		wireNode = powerWireNode;
		outputWireGraph = powerWireGraph;
		outputWire = std::addressof(powerWire);

		// Add as visited objects.
		visited.insert(power->ID());
		visited.insert(powerWire.ID());

		break;
	}
	case sim::CircuitComponentType::CIRCUIT_COMP_GROUND:
	{
		const sim::Ground *ground = circuitComp->asGround();
		const sim::Wire &groundWire = ground->wire();

		sim::CGGround *groundGraph = registeredGround(ground->ID());
		sim::CGWire *groundWireGraph = registeredWire(groundWire.ID());

		groundGraph->setWidth(ground->GetWidth());

		// Ground's tree node.
		sim::CGNode *groundNode = registeredNode(ground->ID(), groundGraph);

		// Wire's value tree node as Ground's child.
		sim::CGNode *groundWireNode = registeredNode(groundWire.ID(), groundWireGraph);

		// Write value from Ground to Ground's Wire object as tree's edge.
		sim::InstructionShared wvGroundToWire =
			sim::CreateWriteValue(groundGraph, groundWireGraph);
		sim::CGEdge groundToWireEdge = { { groundGraph }, { groundWireGraph }, std::move(wvGroundToWire) };
		groundNode->addInstruction(std::move(groundToWireEdge), groundWireNode);

		// Write value from Ground's Wire object to Ground as tree's edge.
		sim::InstructionShared wvWireToGround =
			sim::CreateWriteValue(groundWireGraph, groundGraph);
		sim::CGEdge wireToGroundEdge = { { groundWireGraph }, { groundGraph }, std::move(wvWireToGround) };
		groundWireNode->addInstruction(std::move(wireToGroundEdge), groundNode);

		// Set output's wire to make connections from Ground's Wire object.
		wireNode = groundWireNode;
		outputWireGraph = groundWireGraph;
		outputWire = std::addressof(groundWire);

		// Add as visited objects.
		visited.insert(ground->ID());
		visited.insert(groundWire.ID());

		break;
	}
	case sim::CircuitComponentType::CIRCUIT_COMP_SPLITTER:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_CLOCK:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_TRANSISTOR:
		throw std::logic_error("Not implemented");
	case sim::CircuitComponentType::CIRCUIT_COMP_TRANSMISSION_GATE:
		throw std::logic_error("Not implemented");
	}

	// Make connections from simple Wire object to his adjacent wires as
	// tree's edges.
	for (const sim::wire_t &adjacentWire : outputWire->Wires())
	{
		switch (adjacentWire->wiringComponentType())
		{
		case WIRING_COMP_WIRE:
		{
			// Wire's adjacent wire tree node.
			sim::CGWire *adjacentWireGraph = registeredWire(adjacentWire->ID());
			sim::CGNode *adjacentWireNode = registeredNode(adjacentWire->ID(), adjacentWireGraph);

			// Broadcast value from Pin's Wire to adjacent Wire object as
			// tree's edge.
			sim::InstructionShared wvWireToWire =
				sim::CreateBroadcastValue(outputWireGraph, adjacentWireGraph);
			sim::CGEdge wireToWireEdge = { { outputWireGraph }, { adjacentWireGraph }, std::move(wvWireToWire) };
			wireNode->addInstruction(std::move(wireToWireEdge), adjacentWireNode);

			break;
		}
		case WIRING_COMP_TUNNEL:
			throw std::logic_error("Not implemented.");
		}
		// Add to future BFS visiting.
		bfsVisit.emplace_back(adjacentWire.Ptr());
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
