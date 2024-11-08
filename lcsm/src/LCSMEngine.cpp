#include <initializer_list>
#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Component.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMContext.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit/Constant.h>
#include <lcsm/Model/Circuit/Ground.h>
#include <lcsm/Model/Circuit/Pin.h>
#include <lcsm/Model/Circuit/Power.h>
#include <lcsm/Model/Circuit/Transistor.h>
#include <lcsm/Model/Wiring/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <cstddef>
#include <deque>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::LCSMEngine::LCSMEngine() : m_circuits(0) {}

void lcsm::LCSMEngine::addCircuit(lcsm::LCSMCircuit &circuit)
{
	if (m_circuits != 0)
		throw std::logic_error("");

	m_circuits++;

	std::deque< lcsm::support::PointerView< const lcsm::Component > > bfsVisit;

	for (auto it = circuit.Pins().begin(); it != circuit.Pins().end(); it++)
	{
		const lcsm::model::Pin *pin = it->second->asCircuit()->asPin();

		if (pin->isOutput())
			continue;

		// Initialize future nodes for calculation graph.
		lcsm::CGPin *pinGraph = registeredPinInput(pin->ID());
		lcsm::CGNode *pinNode = registeredStaticNode(pin->ID(), pinGraph);
		m_inputs.addRoot({ pinNode });

		bfsVisit.emplace_back(pin);
	}

	buildCircuit(bfsVisit);
}

std::vector< lcsm::DataBits > lcsm::LCSMEngine::invokeFull(std::initializer_list< lcsm::DataBits >)
{
	return {};
}

lcsm::CGWire *lcsm::LCSMEngine::registerWire(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Wire found same ID.");

	std::shared_ptr< lcsm::CGObject > W = std::make_shared< lcsm::CGWire >();
	m_objects[ID] = W;

	return static_cast< lcsm::CGWire * >(W.get());
}

lcsm::CGPinInput *lcsm::LCSMEngine::registerPinInput(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinInput found same ID.");

	std::shared_ptr< lcsm::CGObject > I = std::make_shared< lcsm::CGPinInput >();
	m_objects[ID] = I;

	return static_cast< lcsm::CGPinInput * >(I.get());
}

lcsm::CGPinOutput *lcsm::LCSMEngine::registerPinOutput(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinOutput found same ID.");

	std::shared_ptr< lcsm::CGObject > O = std::make_shared< lcsm::CGPinOutput >();
	m_objects[ID] = O;

	return static_cast< lcsm::CGPinOutput * >(O.get());
}

lcsm::CGConstant *lcsm::LCSMEngine::registerConstant(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Constant found same ID.");

	std::shared_ptr< lcsm::CGObject > C = std::make_shared< lcsm::CGConstant >();
	m_objects[ID] = C;

	return static_cast< lcsm::CGConstant * >(C.get());
}

lcsm::CGPower *lcsm::LCSMEngine::registerPower(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Power found same ID.");

	std::shared_ptr< lcsm::CGObject > P = std::make_shared< lcsm::CGPower >();
	m_objects[ID] = P;

	return static_cast< lcsm::CGPower * >(P.get());
}

lcsm::CGGround *lcsm::LCSMEngine::registerGround(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Ground found same ID.");

	std::shared_ptr< lcsm::CGObject > G = std::make_shared< lcsm::CGGround >();
	m_objects[ID] = G;

	return static_cast< lcsm::CGGround * >(G.get());
}

lcsm::CGTransistorBase *lcsm::LCSMEngine::registerTransistorBase(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering transistor element found same ID.");

	std::shared_ptr< lcsm::CGObject > T = std::make_shared< lcsm::CGTransistorBase >();
	m_objects[ID] = T;

	return static_cast< lcsm::CGTransistorBase * >(T.get());
}

lcsm::CGTransistorInout *lcsm::LCSMEngine::registerTransistorInout(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering transistor element found same ID.");

	std::shared_ptr< lcsm::CGObject > T = std::make_shared< lcsm::CGTransistorInout >();
	m_objects[ID] = T;

	return static_cast< lcsm::CGTransistorInout * >(T.get());
}

lcsm::CGTransistorState *lcsm::LCSMEngine::registerTransistorState(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering transistor element found same ID.");

	std::shared_ptr< lcsm::CGObject > T = std::make_shared< lcsm::CGTransistorState >();
	m_objects[ID] = T;

	return static_cast< lcsm::CGTransistorState * >(T.get());
}

lcsm::CGWire *lcsm::LCSMEngine::registeredWire(lcsm::Identifier ID)
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

lcsm::CGPinInput *lcsm::LCSMEngine::registeredPinInput(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		lcsm::CGObject *I = found->second.get();
		if (I && I->isPinInput())
			return I->asPinInput();
		else
			throw std::logic_error("RegisteredPinInput");
	}

	return registerPinInput(ID);
}

lcsm::CGPinOutput *lcsm::LCSMEngine::registeredPinOutput(lcsm::Identifier ID)
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

lcsm::CGConstant *lcsm::LCSMEngine::registeredConstant(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		lcsm::CGObject *I = found->second.get();
		if (I && I->isConstant())
			return I->asConstant();
		else
			throw std::logic_error("RegisteredConstant");
	}

	return registerConstant(ID);
}

lcsm::CGPower *lcsm::LCSMEngine::registeredPower(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		lcsm::CGObject *I = found->second.get();
		if (I && I->isPower())
			return I->asPower();
		else
			throw std::logic_error("RegisteredPower");
	}

	return registerPower(ID);
}

lcsm::CGGround *lcsm::LCSMEngine::registeredGround(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		lcsm::CGObject *I = found->second.get();
		if (I && I->isGround())
			return I->asGround();
		else
			throw std::logic_error("RegisteredPower");
	}

	return registerGround(ID);
}

lcsm::CGTransistorBase *lcsm::LCSMEngine::registeredTransistorBase(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		lcsm::CGObject *T = found->second.get();
		if (T && T->isTransistorBase())
			return T->asTransistorBase();
		else
			throw std::logic_error("RegisteredTransistorBase");
	}

	return registerTransistorBase(ID);
}

lcsm::CGTransistorInout *lcsm::LCSMEngine::registeredTransistorInout(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		lcsm::CGObject *T = found->second.get();
		if (T && T->isTransistorInout())
			return T->asTransistorInout();
		else
			throw std::logic_error("RegisteredTransistorInout");
	}

	return registerTransistorInout(ID);
}

lcsm::CGTransistorState *lcsm::LCSMEngine::registeredTransistorState(lcsm::Identifier ID)
{
	const auto found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		lcsm::CGObject *T = found->second.get();
		if (T && T->isTransistorState())
			return T->asTransistorState();
		else
			throw std::logic_error("RegisteredTransistorState");
	}

	return registerTransistorState(ID);
}

lcsm::CGStaticNode *lcsm::LCSMEngine::registeredStaticNode(lcsm::Identifier ID, lcsm::CGObject *object)
{
	const auto found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second->target() == object && found->second->isStatic())
			return found->second->asStatic();
		else
			throw std::logic_error("Static node registered, objects not same");
	}

	std::shared_ptr< lcsm::CGNode > node = std::make_shared< lcsm::CGStaticNode >(object);
	m_nodes[ID] = node;

	return static_cast< lcsm::CGStaticNode * >(node.get());
}

lcsm::CGFastNode *lcsm::LCSMEngine::registeredFastNode(lcsm::Identifier ID, lcsm::CGObject *object)
{
	const auto found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second->target() == object && found->second->isFast())
			return found->second->asFast();
		else
			throw std::logic_error("Static node registered, objects not same");
	}

	std::shared_ptr< lcsm::CGNode > node = std::make_shared< lcsm::CGFastNode >(object);
	m_nodes[ID] = node;

	return static_cast< lcsm::CGFastNode * >(node.get());
}

lcsm::CGCompositeNode *lcsm::LCSMEngine::registeredCompositeNode(lcsm::Identifier ID, lcsm::CGObject *object)
{
	const auto found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second->target() == object && found->second->isComposite())
			return found->second->asComposite();
		else
			throw std::logic_error("Static node registered, objects not same");
	}

	std::shared_ptr< lcsm::CGNode > node = std::make_shared< lcsm::CGCompositeNode >(object);
	m_nodes[ID] = node;

	return static_cast< lcsm::CGCompositeNode * >(node.get());
}

lcsm::CGDynamicNode *lcsm::LCSMEngine::registeredDynamicNode(lcsm::Identifier ID, lcsm::CGObject *object)
{
	const auto found = m_nodes.find(ID);

	if (found != m_nodes.end())
	{
		if (found->second->target() == object && found->second->isDynamic())
			return found->second->asDynamic();
		else
			throw std::logic_error("Static node registered, objects not same");
	}

	std::shared_ptr< lcsm::CGNode > node = std::make_shared< lcsm::CGDynamicNode >(object);
	m_nodes[ID] = node;

	return static_cast< lcsm::CGDynamicNode * >(node.get());
}

void lcsm::LCSMEngine::buildCircuitIOComp(
	std::unordered_set< lcsm::Identifier > &,
	std::deque< lcsm::support::PointerView< const lcsm::Component > > &,
	const lcsm::IOComponent *)
{
	throw std::logic_error("Not implemented");
}

void lcsm::LCSMEngine::buildCircuitWiringComp(
	std::unordered_set< lcsm::Identifier > &visited,
	std::deque< lcsm::support::PointerView< const lcsm::Component > > &bfsVisit,
	const lcsm::WiringComponent *wiringComp)
{
	switch (wiringComp->wiringComponentType())
	{
	case WIRING_COMP_WIRE:
	{
		const lcsm::model::Wire *wire = wiringComp->asWire();
		lcsm::CGWire *wireGraph = registeredWire(wire->ID());

		// Wire's tree node.
		lcsm::CGNode *wireNode = registeredFastNode(wire->ID(), wireGraph);

		// Make connections from Wire object to his adjacent wires as
		// tree's edges.
		for (const lcsm::wire_t &adjacentWire : wire->wires())
		{
			const lcsm::Identifier id = adjacentWire->ID();
			switch (adjacentWire->wiringComponentType())
			{
			case WIRING_COMP_WIRE:
			{
				// Wire's adjacent wire tree node.
				lcsm::CGWire *adjacentWireGraph = registeredWire(id);
				lcsm::CGNode *adjacentWireNode = registeredFastNode(id, adjacentWireGraph);

				// Make connection.
				wireNode->pushBackChild(adjacentWireNode);

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
		for (const lcsm::component_t &comp : wire->connections())
		{
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

void lcsm::LCSMEngine::buildCircuitCircuitComp(
	std::unordered_set< lcsm::Identifier > &visited,
	std::deque< lcsm::support::PointerView< const lcsm::Component > > &bfsVisit,
	const lcsm::CircuitComponent *circuitComp)
{
	switch (circuitComp->circuitComponentType())
	{
	case lcsm::CircuitComponentType::CIRCUIT_COMP_PIN:
	{
		const lcsm::model::Pin *pin = circuitComp->asPin();
		const lcsm::model::Wire &pinWire = pin->wire();

		lcsm::CGPin *pinGraph = nullptr;

		if (pin->isOutput())
			pinGraph = registeredPinOutput(pin->ID());
		else
			pinGraph = registeredPinInput(pin->ID());

		lcsm::CGWire *pinWireGraph = registeredWire(pinWire.ID());

		// Pin's tree node.
		lcsm::CGNode *pinNode = registeredStaticNode(pin->ID(), pinGraph);

		// Wire's tree node as Pin's child.
		lcsm::CGNode *pinWireNode = registeredFastNode(pinWire.ID(), pinWireGraph);

		// Make them to know about each other.
		pinNode->pushBackChild(lcsm::CGNodeView(pinWireNode));
		pinWireNode->pushBackChild(lcsm::CGNodeView(pinNode));

		// Add Pin's wire to queue.
		bfsVisit.emplace_back(std::addressof(pinWire));

		// Add as visited objects.
		visited.insert(pin->ID());

		break;
	}
	case lcsm::CircuitComponentType::CIRCUIT_COMP_CONSTANT:
	{
		const lcsm::model::Constant *constant = circuitComp->asConstant();
		const lcsm::model::Wire &constantWire = constant->wire();

		lcsm::CGConstant *constantGraph = registeredConstant(constant->ID());
		lcsm::CGWire *constantWireGraph = registeredWire(constantWire.ID());

		constantGraph->emplaceDataBits(constant->width(), constant->value());

		// Constant's tree node.
		lcsm::CGNode *constantNode = registeredStaticNode(constant->ID(), constantGraph);

		// Wire's tree node as Constant's child.
		lcsm::CGNode *constantWireNode = registeredFastNode(constantWire.ID(), constantWireGraph);

		// Make them to know about each other.
		constantNode->pushBackChild(lcsm::CGNodeView(constantWireNode));
		constantWireNode->pushBackChild(lcsm::CGNodeView(constantNode));

		// Add Constant's wire to queue.
		bfsVisit.emplace_back(std::addressof(constantWire));

		// Add as visited objects.
		visited.insert(constant->ID());

		break;
	}
	case lcsm::CircuitComponentType::CIRCUIT_COMP_POWER:
	{
		const lcsm::model::Power *power = circuitComp->asPower();
		const lcsm::model::Wire &powerWire = power->wire();

		lcsm::CGPower *powerGraph = registeredPower(power->ID());
		lcsm::CGWire *powerWireGraph = registeredWire(powerWire.ID());

		powerGraph->setWidth(power->width());

		// Power's tree node.
		lcsm::CGNode *powerNode = registeredStaticNode(power->ID(), powerGraph);

		// Wire's value tree node as Power's child.
		lcsm::CGNode *powerWireNode = registeredFastNode(powerWire.ID(), powerWireGraph);

		// Make them to know about each other.
		powerNode->pushBackChild(lcsm::CGNodeView(powerWireNode));
		powerWireNode->pushBackChild(lcsm::CGNodeView(powerNode));

		// Add Power's wire to queue.
		bfsVisit.emplace_back(std::addressof(powerWire));

		// Add as visited objects.
		visited.insert(power->ID());

		break;
	}
	case lcsm::CircuitComponentType::CIRCUIT_COMP_GROUND:
	{
		const lcsm::model::Ground *ground = circuitComp->asGround();
		const lcsm::model::Wire &groundWire = ground->wire();

		lcsm::CGGround *groundGraph = registeredGround(ground->ID());
		lcsm::CGWire *groundWireGraph = registeredWire(groundWire.ID());

		groundGraph->setWidth(ground->width());

		// Ground's tree node.
		lcsm::CGNode *groundNode = registeredStaticNode(ground->ID(), groundGraph);

		// Wire's value tree node as Ground's child.
		lcsm::CGNode *groundWireNode = registeredFastNode(groundWire.ID(), groundWireGraph);

		// Make them to know about each other.
		groundNode->pushBackChild(lcsm::CGNodeView(groundWireNode));
		groundWireNode->pushBackChild(lcsm::CGNodeView(groundNode));

		// Add Ground's wire to queue.
		bfsVisit.emplace_back(std::addressof(groundWire));

		// Add as visited objects.
		visited.insert(ground->ID());

		break;
	}
	case lcsm::CircuitComponentType::CIRCUIT_COMP_SPLITTER:
		throw std::logic_error("Not implemented");
	case lcsm::CircuitComponentType::CIRCUIT_COMP_CLOCK:
		throw std::logic_error("Not implemented");
	case lcsm::CircuitComponentType::CIRCUIT_COMP_TRANSISTOR:
	{
		const lcsm::model::Transistor *transistor = circuitComp->asTransistor();
		const lcsm::model::Wire &wireBase = transistor->wireBase();
		const lcsm::model::Wire &wireSrcA = transistor->wireInoutA();
		const lcsm::model::Wire &wireSrcB = transistor->wireInoutB();

		lcsm::CGWire *transistorWireBaseGraph = registeredWire(wireBase.ID());
		lcsm::CGWire *transistorWireSrcAGraph = registeredWire(wireSrcA.ID());
		lcsm::CGWire *transistorWireSrcBGraph = registeredWire(wireSrcB.ID());

		lcsm::CGTransistorBase *transistorBaseGraph = registeredTransistorBase(transistor->idBase());
		lcsm::CGTransistorInout *transistorSrcAGraph = registeredTransistorInout(transistor->idInoutA());
		lcsm::CGTransistorInout *transistorSrcBGraph = registeredTransistorInout(transistor->idInoutB());
		lcsm::CGTransistorState *transistorStateGraph = registeredTransistorState(transistor->ID());

		// Base's Wire tree node.
		lcsm::CGNode *transistorWireBaseNode = registeredFastNode(wireBase.ID(), transistorWireBaseGraph);

		// SrcA's Wire tree node.
		lcsm::CGNode *transistorWireSrcANode = registeredFastNode(wireSrcA.ID(), transistorWireSrcAGraph);

		// SrcB's Wire tree node.
		lcsm::CGNode *transistorWireSrcBNode = registeredFastNode(wireSrcB.ID(), transistorWireSrcBGraph);

		// Base's tree node.
		lcsm::CGNode *transistorBaseNode = registeredCompositeNode(transistor->idBase(), transistorBaseGraph);

		// SrcA's tree node.
		lcsm::CGNode *transistorSrcANode = registeredCompositeNode(transistor->idInoutA(), transistorSrcAGraph);

		// SrcB's tree node.
		lcsm::CGNode *transistorSrcBNode = registeredCompositeNode(transistor->idInoutB(), transistorWireSrcBGraph);

		// State's tree node.
		lcsm::CGNode *transistorStateNode = registeredDynamicNode(transistor->ID(), transistorStateGraph);

		// Make them to know about each other.
		const lcsm::CGNodeView transistorBaseNodeView = transistorBaseNode;
		const lcsm::CGNodeView transistorSrcANodeView = transistorSrcANode;
		const lcsm::CGNodeView transistorSrcBNodeView = transistorSrcBNode;

		transistorBaseNode->pushBackChild(lcsm::CGNodeView(transistorWireBaseNode));
		transistorWireBaseNode->pushBackChild(transistorBaseNodeView);
		transistorSrcANode->pushBackChild(lcsm::CGNodeView(transistorWireSrcANode));
		transistorWireSrcANode->pushBackChild(transistorSrcANodeView);
		transistorSrcBNode->pushBackChild(lcsm::CGNodeView(transistorWireSrcBNode));
		transistorWireSrcBNode->pushBackChild(transistorSrcBNodeView);

		transistorStateNode->pushBackChild(transistorBaseNodeView);
		transistorStateNode->pushBackChild(transistorSrcANodeView);
		transistorStateNode->pushBackChild(transistorSrcBNodeView);

		// Make transistor's inouts and base to know about state object.
		const lcsm::support::PointerView< lcsm::CGState > stateView = transistorStateGraph;
		transistorBaseGraph->setState(stateView);
		transistorSrcAGraph->setState(stateView);
		transistorSrcBGraph->setState(stateView);

		// Add all Transistor's wire to queue.
		bfsVisit.emplace_back(std::addressof(wireBase));
		bfsVisit.emplace_back(std::addressof(wireSrcA));
		bfsVisit.emplace_back(std::addressof(wireSrcB));

		// Add as visited objects.
		visited.insert(transistor->ID());
		visited.insert(transistor->idBase());
		visited.insert(transistor->idInoutA());
		visited.insert(transistor->idInoutB());

		break;
	}
	case lcsm::CircuitComponentType::CIRCUIT_COMP_TRANSMISSION_GATE:
		throw std::logic_error("Not implemented");
	}
}

void lcsm::LCSMEngine::buildCircuit(std::deque< lcsm::support::PointerView< const lcsm::Component > > &bfsVisit)
{
	std::unordered_set< lcsm::Identifier > visited;

	while (!bfsVisit.empty())
	{
		std::size_t levelSize = bfsVisit.size();

		while (levelSize != 0)
		{
			levelSize--;

			lcsm::support::PointerView< const lcsm::Component > comp = bfsVisit.front();
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
