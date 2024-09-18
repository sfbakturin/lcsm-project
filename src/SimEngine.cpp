#include <initializer_list>
#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Component.h>
#include <sim/Component/Identifier.h>
#include <sim/Component/WiringComponent.h>
#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Circuit/Ground.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/Model/Circuit/Power.h>
#include <sim/Model/Circuit/Transistor.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/SimCircuit.h>
#include <sim/SimContext.h>
#include <sim/SimEngine.h>
#include <sim/SimState.h>
#include <sim/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <cstddef>
#include <deque>
#include <memory>
#include <stdexcept>
#include <tuple>
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
		const sim::Pin *pin = it->second->asCircuit()->asPin();

		if (pin->isOutput())
			continue;

		// Initialize future nodes for calculation graph.
		sim::CGPin *pinGraph = registeredPinInput(pin->ID());
		sim::CGNode *pinNode = registeredStaticNode(pin->ID(), pinGraph);
		m_inputs.addRoot({ pinNode });

		bfsVisit.emplace_back(pin);
	}

	buildCircuit(bfsVisit);
}

std::vector< sim::Value > sim::SimEngine::invokeFull(std::initializer_list< sim::Value >)
{
	return {};
}

sim::CGWire *sim::SimEngine::registerWire(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Wire found same ID.");

	std::shared_ptr< sim::CGObject > W = std::make_shared< sim::CGWire >();
	m_objects[ID] = W;

	return static_cast< sim::CGWire * >(W.get());
}

sim::CGPinInput *sim::SimEngine::registerPinInput(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinInput found same ID.");

	std::shared_ptr< sim::CGObject > I = std::make_shared< sim::CGPinInput >();
	m_objects[ID] = I;

	return static_cast< sim::CGPinInput * >(I.get());
}

sim::CGPinOutput *sim::SimEngine::registerPinOutput(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinOutput found same ID.");

	std::shared_ptr< sim::CGObject > O = std::make_shared< sim::CGPinOutput >();
	m_objects[ID] = O;

	return static_cast< sim::CGPinOutput * >(O.get());
}

sim::CGConstant *sim::SimEngine::registerConstant(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Constant found same ID.");

	std::shared_ptr< sim::CGObject > C = std::make_shared< sim::CGConstant >();
	m_objects[ID] = C;

	return static_cast< sim::CGConstant * >(C.get());
}

sim::CGPower *sim::SimEngine::registerPower(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Power found same ID.");

	std::shared_ptr< sim::CGObject > P = std::make_shared< sim::CGPower >();
	m_objects[ID] = P;

	return static_cast< sim::CGPower * >(P.get());
}

sim::CGGround *sim::SimEngine::registerGround(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Ground found same ID.");

	std::shared_ptr< sim::CGObject > G = std::make_shared< sim::CGGround >();
	m_objects[ID] = G;

	return static_cast< sim::CGGround * >(G.get());
}

sim::CGTransistorBase *sim::SimEngine::registerTransistorBase(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering transistor element found same ID.");

	std::shared_ptr< sim::CGObject > T = std::make_shared< sim::CGTransistorBase >();
	m_objects[ID] = T;

	return static_cast< sim::CGTransistorBase * >(T.get());
}

sim::CGTransistorInout *sim::SimEngine::registerTransistorInout(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering transistor element found same ID.");

	std::shared_ptr< sim::CGObject > T = std::make_shared< sim::CGTransistorInout >();
	m_objects[ID] = T;

	return static_cast< sim::CGTransistorInout * >(T.get());
}

sim::CGTransistorState *sim::SimEngine::registerTransistorState(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering transistor element found same ID.");

	std::shared_ptr< sim::CGObject > T = std::make_shared< sim::CGTransistorState >();
	m_objects[ID] = T;

	return static_cast< sim::CGTransistorState * >(T.get());
}

sim::CGWire *sim::SimEngine::registeredWire(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

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

sim::CGPinInput *sim::SimEngine::registeredPinInput(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

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

sim::CGPinOutput *sim::SimEngine::registeredPinOutput(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

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

sim::CGConstant *sim::SimEngine::registeredConstant(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

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

sim::CGPower *sim::SimEngine::registeredPower(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

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

sim::CGGround *sim::SimEngine::registeredGround(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

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

sim::CGTransistorBase *sim::SimEngine::registeredTransistorBase(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		sim::CGObject *T = found->second.get();
		if (T && T->isTransistorBase())
			return T->asTransistorBase();
		else
			throw std::logic_error("RegisteredTransistorBase");
	}

	return registerTransistorBase(ID);
}

sim::CGTransistorInout *sim::SimEngine::registeredTransistorInout(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		sim::CGObject *T = found->second.get();
		if (T && T->isTransistorInout())
			return T->asTransistorInout();
		else
			throw std::logic_error("RegisteredTransistorInout");
	}

	return registerTransistorInout(ID);
}

sim::CGTransistorState *sim::SimEngine::registeredTransistorState(sim::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		sim::CGObject *T = found->second.get();
		if (T && T->isTransistorState())
			return T->asTransistorState();
		else
			throw std::logic_error("RegisteredTransistorState");
	}

	return registerTransistorState(ID);
}

sim::CGStaticNode *sim::SimEngine::registeredStaticNode(sim::Identifier ID, sim::CGObject *object)
{
	const auto found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second->object() == object && found->second->isStatic())
			return found->second->asStatic();
		else
			throw std::logic_error("Static node registered, objects not same");
	}

	std::shared_ptr< sim::CGNode > node = std::make_shared< sim::CGStaticNode >(object);
	m_nodes[ID] = node;

	return static_cast< sim::CGStaticNode * >(node.get());
}

sim::CGFastNode *sim::SimEngine::registeredFastNode(sim::Identifier ID, sim::CGObject *object)
{
	const auto found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second->object() == object && found->second->isFast())
			return found->second->asFast();
		else
			throw std::logic_error("Static node registered, objects not same");
	}

	std::shared_ptr< sim::CGNode > node = std::make_shared< sim::CGFastNode >(object);
	m_nodes[ID] = node;

	return static_cast< sim::CGFastNode * >(node.get());
}

sim::CGCompositeNode *
	sim::SimEngine::registeredCompositeNode(sim::Identifier ID, sim::CGObject *object)
{
	const auto found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second->object() == object && found->second->isComposite())
			return found->second->asComposite();
		else
			throw std::logic_error("Static node registered, objects not same");
	}

	std::shared_ptr< sim::CGNode > node = std::make_shared< sim::CGCompositeNode >(object);
	m_nodes[ID] = node;

	return static_cast< sim::CGCompositeNode * >(node.get());
}

sim::CGDynamicNode *
	sim::SimEngine::registeredDynamicNode(sim::Identifier ID, sim::CGObject *object)
{
	const auto found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second->object() == object && found->second->isDynamic())
			return found->second->asDynamic();
		else
			throw std::logic_error("Static node registered, objects not same");
	}

	std::shared_ptr< sim::CGNode > node = std::make_shared< sim::CGDynamicNode >(object);
	m_nodes[ID] = node;

	return static_cast< sim::CGDynamicNode * >(node.get());
}

void sim::SimEngine::buildCircuitIOComp(
	std::unordered_set< sim::Identifier > &,
	std::deque< sim::support::PointerView< const sim::Component > > &,
	const sim::IOComponent *)
{
	throw std::logic_error("Not implemented");
}

void sim::SimEngine::buildCircuitWiringComp(
	std::unordered_set< sim::Identifier > &visited,
	std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
	const sim::WiringComponent *wiringComp)
{
	switch (wiringComp->wiringComponentType())
	{
	case WIRING_COMP_WIRE:
	{
		const sim::Wire *wire = wiringComp->asWire();
		sim::CGWire *wireGraph = registeredWire(wire->ID());

		// Wire's tree node.
		sim::CGNode *wireNode = registeredFastNode(wire->ID(), wireGraph);

		// Make connections from Wire object to his adjacent wires as
		// tree's edges.
		for (const sim::wire_t &adjacentWire : wire->wires())
		{
			switch (adjacentWire->wiringComponentType())
			{
			case WIRING_COMP_WIRE:
			{
				// Wire's adjacent wire tree node.
				sim::CGWire *adjacentWireGraph = registeredWire(adjacentWire->ID());
				sim::CGNode *adjacentWireNode =
					registeredFastNode(adjacentWire->ID(), adjacentWireGraph);

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
			bfsVisit.emplace_back(adjacentWire.cptr());
		}

		// This wire might be some object's wire, so we should make connection
		// as tree's edge.
		for (const sim::component_t &comp : wire->connections())
		{
			switch (comp->componentType())
			{
			case COMP_IO:
				throw std::logic_error("Not implemented");
			case COMP_WIRING:
				throw std::logic_error("Impossible");
			case COMP_CIRCUIT:
			{
				const sim::CircuitComponent *circComp = comp->asCircuit();
				switch (circComp->circuitComponentType())
				{
				case CIRCUIT_COMP_PIN:
				{
					const sim::Pin *pin = circComp->asPin();
					sim::CGPin *pinGraph = nullptr;

					if (pin->isOutput())
						pinGraph = registeredPinOutput(pin->ID());
					else
						pinGraph = registeredPinInput(pin->ID());

					// Make connection between Pin's Wire to Pin object as tree
					// node.
					sim::CGNode *pinNode = registeredStaticNode(pin->ID(), pinGraph);

					sim::InstructionShared wvWireToPin =
						sim::CreateWriteValue(wireGraph, pinGraph);
					sim::CGEdge wireToPinEdge = { { wireGraph }, { pinGraph }, std::move(wvWireToPin) };
					wireNode->addInstruction(std::move(wireToPinEdge), pinNode);

					break;
				}
				case CIRCUIT_COMP_CONSTANT:
				{
					const sim::Constant *constant = circComp->asConstant();
					sim::CGConstant *constantGraph = registerConstant(constant->ID());

					// Make connection between Constant's Wire to Constant
					// object as tree node.
					sim::CGNode *constantNode =
						registeredStaticNode(constant->ID(), constantGraph);

					sim::InstructionShared wvWireToConstant =
						sim::CreateWriteValue(wireGraph, constantGraph);
					sim::CGEdge wireToConstantEdge = { { wireGraph }, { constantGraph }, std::move(wvWireToConstant) };
					wireNode->addInstruction(std::move(wireToConstantEdge), constantNode);

					break;
				}
				case CIRCUIT_COMP_POWER:
				{
					const sim::Power *power = circComp->asPower();
					sim::CGPower *powerGraph = registeredPower(power->ID());

					// Make connection between Power's Wire to Power
					// object as tree node.
					sim::CGNode *powerNode = registeredStaticNode(power->ID(), powerGraph);

					sim::InstructionShared wvWireToPower =
						sim::CreateWriteValue(wireGraph, powerGraph);
					sim::CGEdge wireToPowerEdge = { { wireGraph }, { powerGraph }, std::move(wvWireToPower) };
					wireNode->addInstruction(std::move(wireToPowerEdge), powerNode);

					break;
				}
				case CIRCUIT_COMP_GROUND:
				{
					const sim::Ground *ground = circComp->asGround();
					sim::CGGround *groundGraph = registeredGround(ground->ID());

					// Make connection between Ground's Wire to Ground
					// object as tree node.
					sim::CGNode *groundNode = registeredStaticNode(ground->ID(), groundGraph);

					sim::InstructionShared wvWireToGround =
						sim::CreateWriteValue(wireGraph, groundGraph);
					sim::CGEdge wireToGroundEdge = { { wireGraph }, { groundGraph }, std::move(wvWireToGround) };
					wireNode->addInstruction(std::move(wireToGroundEdge), groundNode);

					break;
				}
				case CIRCUIT_COMP_SPLITTER:
					throw std::logic_error("Not implemented");
				case CIRCUIT_COMP_CLOCK:
					throw std::logic_error("Not implemented");
				case CIRCUIT_COMP_TRANSISTOR:
				{
					const sim::Transistor *transistor = circComp->asTransistor();
					sim::CGObject *transistorElementGraph = nullptr;
					sim::Identifier elementId;

					switch (transistor->testConnectivity(wire))
					{
					case sim::Transistor::CompositeIndex::BASE:
						elementId = transistor->idBase();
						transistorElementGraph = registeredTransistorBase(elementId);
						break;
					case sim::Transistor::CompositeIndex::INOUT_A:
						elementId = transistor->idInoutA();
						transistorElementGraph = registeredTransistorInout(elementId);
						break;
					case sim::Transistor::CompositeIndex::INOUT_B:
						elementId = transistor->idInoutB();
						transistorElementGraph = registeredTransistorInout(elementId);
						break;
					default:
						throw std::logic_error("");
					}

					// Make connection between Transistor element's Wire to
					// Transistor element object as tree node.
					sim::CGNode *transistorElementNode =
						registeredCompositeNode(elementId, transistorElementGraph);

					sim::InstructionShared wvWireToInout =
						sim::CreateWriteValue(wireGraph, transistorElementGraph);
					sim::CGEdge wireToElementEdge = {
						{ wireGraph },
						{ transistorElementGraph },
						std::move(wvWireToInout)
					};
					wireNode->addInstruction(std::move(wireToElementEdge), transistorElementNode);

					break;
				}
				case CIRCUIT_COMP_TRANSMISSION_GATE:
					throw std::logic_error("Not implemented");
				}
			}
			}
			// Add to future BFS visiting.
			bfsVisit.emplace_back(comp.cptr());
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
	std::unordered_set< sim::Identifier > &visited,
	std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit,
	const sim::CircuitComponent *circuitComp)
{
	static constexpr std::size_t IDX_NODE = 0;
	static constexpr std::size_t IDX_GRAPH = 1;
	static constexpr std::size_t IDX_OUTPUT = 2;

	using node_t = sim::support::PointerView< sim::CGNode >;
	using graph_t = sim::support::PointerView< sim::CGWire >;
	using output_t = sim::support::PointerView< const sim::Wire >;

	std::vector< std::tuple< node_t, graph_t, output_t > > outputs;

	switch (circuitComp->circuitComponentType())
	{
	case sim::CircuitComponentType::CIRCUIT_COMP_PIN:
	{
		const sim::Pin *pin = circuitComp->asPin();
		const sim::Wire &pinWire = pin->wire();

		sim::CGPin *pinGraph = nullptr;

		if (pin->isOutput())
			pinGraph = registeredPinOutput(pin->ID());
		else
			pinGraph = registeredPinInput(pin->ID());

		sim::CGWire *pinWireGraph = registeredWire(pinWire.ID());

		// Pin's tree node.
		sim::CGNode *pinNode = registeredStaticNode(pin->ID(), pinGraph);

		// Wire's tree node as Pin's child.
		sim::CGNode *pinWireNode = registeredFastNode(pinWire.ID(), pinWireGraph);

		// Write value from Pin to Pin's Wire object as tree's edge.
		sim::InstructionShared wvPinToWire = sim::CreateWriteValue(pinGraph, pinWireGraph);
		sim::CGEdge pinToWireEdge = { { pinGraph }, { pinWireGraph }, std::move(wvPinToWire) };
		pinNode->addInstruction(std::move(pinToWireEdge), pinWireNode);

		// Write value from Pin's Wire object to Pin as tree's edge.
		sim::InstructionShared wvWireToPin = sim::CreateWriteValue(pinWireGraph, pinGraph);
		sim::CGEdge wireToPinEdge = { { pinWireGraph }, { pinGraph }, std::move(wvWireToPin) };
		pinWireNode->addInstruction(std::move(wireToPinEdge), pinNode);

		// Set output's wire to make connections from Pin's Wire object.
		outputs.emplace_back(pinWireNode, pinWireGraph, pinWire);

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

		constantGraph->emplaceValue(constant->width(), constant->value());

		// Constant's tree node.
		sim::CGNode *constantNode = registeredStaticNode(constant->ID(), constantGraph);

		// Wire's tree node as Constant's child.
		sim::CGNode *constantWireNode = registeredFastNode(constantWire.ID(), constantWireGraph);

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
		outputs.emplace_back(constantWireNode, constantWireGraph, constantWire);

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

		powerGraph->setWidth(power->width());

		// Power's tree node.
		sim::CGNode *powerNode = registeredStaticNode(power->ID(), powerGraph);

		// Wire's value tree node as Power's child.
		sim::CGNode *powerWireNode = registeredFastNode(powerWire.ID(), powerWireGraph);

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
		outputs.emplace_back(powerWireNode, powerWireGraph, powerWire);

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

		groundGraph->setWidth(ground->width());

		// Ground's tree node.
		sim::CGNode *groundNode = registeredStaticNode(ground->ID(), groundGraph);

		// Wire's value tree node as Ground's child.
		sim::CGNode *groundWireNode = registeredFastNode(groundWire.ID(), groundWireGraph);

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
		outputs.emplace_back(groundWireNode, groundWireGraph, groundWire);

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
	{
		const sim::Transistor *transistor = circuitComp->asTransistor();
		const sim::Wire &wireBase = transistor->wireBase();
		const sim::Wire &wireSrcA = transistor->wireInoutA();
		const sim::Wire &wireSrcB = transistor->wireInoutB();

		sim::CGWire *transistorWireBaseGraph = registeredWire(wireBase.ID());
		sim::CGWire *transistorWireSrcAGraph = registeredWire(wireSrcA.ID());
		sim::CGWire *transistorWireSrcBGraph = registeredWire(wireSrcB.ID());

		sim::CGTransistorBase *transistorBaseGraph =
			registeredTransistorBase(transistor->idBase());
		sim::CGTransistorInout *transistorSrcAGraph =
			registeredTransistorInout(transistor->idInoutA());
		sim::CGTransistorInout *transistorSrcBGraph =
			registeredTransistorInout(transistor->idInoutB());
		sim::CGTransistorState *transistorStateGraph =
			registeredTransistorState(transistor->ID());

		// Base's Wire tree node.
		sim::CGNode *transistorWireBaseNode =
			registeredFastNode(wireBase.ID(), transistorWireBaseGraph);

		// SrcA's Wire tree node.
		sim::CGNode *transistorWireSrcANode =
			registeredFastNode(wireSrcA.ID(), transistorWireSrcAGraph);

		// SrcB's Wire tree node.
		sim::CGNode *transistorWireSrcBNode =
			registeredFastNode(wireSrcB.ID(), transistorWireSrcBGraph);

		// Base's tree node.
		sim::CGNode *transistorBaseNode =
			registeredCompositeNode(transistor->idBase(), transistorBaseGraph);

		// SrcA's tree node.
		sim::CGNode *transistorSrcANode =
			registeredCompositeNode(transistor->idInoutA(), transistorSrcAGraph);

		// SrcB's tree node.
		sim::CGNode *transistorSrcBNode =
			registeredCompositeNode(transistor->idInoutB(), transistorWireSrcBGraph);

		// State's tree node.
		sim::CGNode *transistorStateNode =
			registeredDynamicNode(transistor->ID(), transistorStateGraph);

		// Write value from Base's Wire to Base's tree node.
		sim::InstructionShared wvBaseWireToBase =
			sim::CreateWriteValue(transistorWireBaseGraph, transistorBaseGraph);
		sim::CGEdge baseWireToBase = {
			{ transistorWireBaseGraph },
			{ transistorBaseGraph },
			std::move(wvBaseWireToBase)
		};
		transistorWireBaseNode->addInstruction(std::move(baseWireToBase), transistorBaseNode);

		// Write value from Base's to Base's Wire tree node.
		sim::InstructionShared wvBaseToBaseWire =
			sim::CreateWriteValue(transistorBaseGraph, transistorWireBaseGraph);
		sim::CGEdge baseToBaseWire = {
			{ transistorBaseGraph },
			{ transistorWireBaseGraph },
			std::move(wvBaseToBaseWire)
		};
		transistorBaseNode->addInstruction(std::move(baseToBaseWire), transistorWireBaseNode);

		/* Instruction: write value from SrcA's Wire to SrcA's tree node. */
		sim::InstructionShared wvSrcWireToSrc =
			sim::CreateWriteValue(transistorWireSrcAGraph, transistorSrcAGraph);
		sim::CGEdge srcWireToSrc = { { transistorWireSrcAGraph }, { transistorSrcAGraph }, std::move(wvSrcWireToSrc) };
		transistorWireSrcANode->addInstruction(std::move(srcWireToSrc), transistorSrcANode);

		/* Instruction: write value from SrcA's to SrcA's Wire tree node. */
		sim::InstructionShared wvSrcToSrcWire =
			sim::CreateWriteValue(transistorSrcAGraph, transistorWireSrcAGraph);
		sim::CGEdge srcToSrcWire = { { transistorSrcAGraph }, { transistorWireSrcAGraph }, std::move(wvSrcToSrcWire) };
		transistorSrcANode->addInstruction(std::move(srcToSrcWire), transistorWireSrcANode);

		/* Instruction: write value from SrcB's Wire to SrcB's tree node. */
		wvSrcWireToSrc = sim::CreateWriteValue(transistorWireSrcBGraph, transistorSrcBGraph);
		srcWireToSrc = { { transistorWireSrcBGraph }, { transistorSrcBGraph }, std::move(wvSrcWireToSrc) };
		transistorWireSrcBNode->addInstruction(std::move(srcWireToSrc), transistorSrcBNode);

		/* Instruction: write value from SrcB's to SrcB's Wire tree node. */
		wvSrcToSrcWire = sim::CreateWriteValue(transistorSrcBGraph, transistorWireSrcBGraph);
		srcToSrcWire = { { transistorSrcBGraph }, { transistorWireSrcBGraph }, std::move(wvSrcToSrcWire) };
		transistorSrcBNode->addInstruction(std::move(srcToSrcWire), transistorWireSrcBNode);

		/* Instruction: make request from Base's for updating state for
		 * Transistor's State.*/
		sim::InstructionShared updateState = sim::CreateUpdateState(transistorStateGraph);
		sim::CGEdge baseToState = { { transistorBaseGraph }, { transistorStateGraph }, std::move(updateState) };
		transistorBaseNode->addInstruction(std::move(baseToState), transistorStateNode);

		/* Instruction: make request from SrcA's for updating state for
		 * Transistor's State.*/
		updateState = sim::CreateUpdateState(transistorStateGraph);
		sim::CGEdge srcToState = { { transistorSrcAGraph }, { transistorStateGraph }, std::move(updateState) };
		transistorSrcANode->addInstruction(std::move(srcToState), transistorStateNode);

		/* Instruction: make request from SrcB's for updating state for
		 * Transistor's State.*/
		updateState = sim::CreateUpdateState(transistorStateGraph);
		srcToState = { { transistorSrcBGraph }, { transistorStateGraph }, std::move(updateState) };
		transistorSrcBNode->addInstruction(std::move(srcToState), transistorStateNode);

		// Set output's wire to make connections from Transistor's Wires object.
		outputs.emplace_back(transistorWireBaseNode, transistorWireBaseGraph, wireBase);
		outputs.emplace_back(transistorWireSrcANode, transistorWireSrcAGraph, wireSrcA);
		outputs.emplace_back(transistorWireSrcBNode, transistorWireSrcBGraph, wireSrcB);

		// Add as visited objects.
		visited.insert(transistor->ID());
		visited.insert(transistor->idBase());
		visited.insert(transistor->idInoutA());
		visited.insert(transistor->idInoutB());
		visited.insert(wireBase.ID());
		visited.insert(wireSrcA.ID());
		visited.insert(wireSrcB.ID());

		break;
	}
	case sim::CircuitComponentType::CIRCUIT_COMP_TRANSMISSION_GATE:
		throw std::logic_error("Not implemented");
	}

	// Make connections from simple Wire object to his adjacent wires as
	// tree's edges.
	for (auto o : outputs)
	{
		node_t &node = std::get< IDX_NODE >(o);
		graph_t &graph = std::get< IDX_GRAPH >(o);
		output_t &output = std::get< IDX_OUTPUT >(o);

		node_t::pointer wireNode = node.ptr();
		graph_t::pointer outputWireGraph = graph.ptr();
		output_t::const_pointer outputWire = output.ptr();

		for (const sim::wire_t &adjacentWire : outputWire->wires())
		{
			switch (adjacentWire->wiringComponentType())
			{
			case WIRING_COMP_WIRE:
			{
				// Wire's adjacent wire tree node.
				sim::CGWire *adjacentWireGraph = registeredWire(adjacentWire->ID());
				sim::CGNode *adjacentWireNode =
					registeredFastNode(adjacentWire->ID(), adjacentWireGraph);

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
			bfsVisit.emplace_back(adjacentWire.cptr());
		}
	}
}

void sim::SimEngine::buildCircuit(std::deque< sim::support::PointerView< const sim::Component > > &bfsVisit)
{
	std::unordered_set< sim::Identifier > visited;

	while (!bfsVisit.empty())
	{
		std::size_t levelSize = bfsVisit.size();

		while (levelSize != 0)
		{
			levelSize--;

			sim::support::PointerView< const sim::Component > comp = bfsVisit.front();
			bfsVisit.pop_front();

			const auto found = visited.find(comp->ID());
			if (found != visited.cend())
				continue;

			switch (comp->componentType())
			{
			case COMP_IO:
				buildCircuitIOComp(visited, bfsVisit, comp->asIO());
				break;
			case COMP_WIRING:
				buildCircuitWiringComp(visited, bfsVisit, comp->asWiring());
				break;
			case COMP_CIRCUIT:
				buildCircuitCircuitComp(visited, bfsVisit, comp->asCircuit());
				break;
			}
		}
	}
}
