#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/std/Constant.h>
#include <lcsm/Physical/std/Ground.h>
#include <lcsm/Physical/std/PinInput.h>
#include <lcsm/Physical/std/PinOutput.h>
#include <lcsm/Physical/std/Power.h>
#include <lcsm/Physical/std/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <cstddef>
#include <deque>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::LCSMEngine lcsm::LCSMEngine::fromCircuit(const lcsm::LCSMCircuit &circuit)
{
	// Generated calculation graph.
	lcsm::LCSMEngine engine;

	// BFS in-future visited components.
	std::deque< lcsm::support::PointerView< const lcsm::Circuit > > visit;

	// Components.
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &components = circuit.components();

	// Find all *In*/*Out* circuits and add them to BFS queue.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		lcsm::support::PointerView< lcsm::Circuit > circ = lcsm::support::PointerView< lcsm::Circuit >::fromSharedPtr(it->second);
		switch (circ->objectType())
		{
		case lcsm::ObjectType::IntExtIn:
		case lcsm::ObjectType::IntExtOut:
			visit.emplace_back(circ.cptr());
			break;
		default:
			break;
		}
	}

	engine.buildCircuit(visit);

	return engine;
}

lcsm::LCSMState lcsm::LCSMEngine::fork()
{
	lcsm::LCSMState state = m_objects;
	return state;
}

/*
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

	std::shared_ptr< lcsm::CGObject > W = std::make_shared< lcsm::CGWire >();
	m_objects[ID] = W;

	return static_cast< lcsm::CGWire * >(W.get());
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

	std::shared_ptr< lcsm::CGObject > I = std::make_shared< lcsm::CGPinInput >();
	m_objects[ID] = I;

	return static_cast< lcsm::CGPinInput * >(I.get());
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

	std::shared_ptr< lcsm::CGObject > O = std::make_shared< lcsm::CGPinOutput >();
	m_objects[ID] = O;

	return static_cast< lcsm::CGPinOutput * >(O.get());
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

	std::shared_ptr< lcsm::CGObject > C = std::make_shared< lcsm::CGConstant >();
	m_objects[ID] = C;

	return static_cast< lcsm::CGConstant * >(C.get());
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

	std::shared_ptr< lcsm::CGObject > P = std::make_shared< lcsm::CGPower >();
	m_objects[ID] = P;

	return static_cast< lcsm::CGPower * >(P.get());
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

	std::shared_ptr< lcsm::CGObject > G = std::make_shared< lcsm::CGGround >();
	m_objects[ID] = G;

	return static_cast< lcsm::CGGround * >(G.get());
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

	std::shared_ptr< lcsm::CGObject > T = std::make_shared< lcsm::CGTransistorBase >();
	m_objects[ID] = T;

	return static_cast< lcsm::CGTransistorBase * >(T.get());
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

	std::shared_ptr< lcsm::CGObject > T = std::make_shared< lcsm::CGTransistorInout >();
	m_objects[ID] = T;

	return static_cast< lcsm::CGTransistorInout * >(T.get());
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

	std::shared_ptr< lcsm::CGObject > T = std::make_shared< lcsm::CGTransistorState >();
	m_objects[ID] = T;

	return static_cast< lcsm::CGTransistorState * >(T.get());
}
*/

/*
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
		lcsm::EvaluatorNode *wireNode = static_cast< lcsm::EvaluatorNode * >(wireGraph);

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
				lcsm::EvaluatorNode *adjacentWireNode = static_cast< lcsm::EvaluatorNode * >(adjacentWireGraph);

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

		if (pin->output())
			pinGraph = registeredPinOutput(pin->ID());
		else
			pinGraph = registeredPinInput(pin->ID());

		lcsm::CGWire *pinWireGraph = registeredWire(pinWire.ID());

		// Pin's tree node.
		lcsm::EvaluatorNode *pinNode = static_cast< lcsm::EvaluatorNode * >(pinGraph);

		// Wire's tree node as Pin's child.
		lcsm::EvaluatorNode *pinWireNode = static_cast< lcsm::EvaluatorNode * >(pinWireGraph);

		// Make them to know about each other.
		pinNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(pinWireNode));
		pinWireNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(pinNode));

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
		const lcsm::Identifier id = constant->ID();

		lcsm::CGConstant *constantGraph = registeredConstant(id);
		lcsm::CGWire *constantWireGraph = registeredWire(constantWire.ID());

		// Save constants.
		m_contants[id] = lcsm::DataBits::fromModel(constant->width(), constant->value());

		// Constant's tree node.
		lcsm::EvaluatorNode *constantNode = static_cast< lcsm::EvaluatorNode * >(constantGraph);

		// Wire's tree node as Constant's child.
		lcsm::EvaluatorNode *constantWireNode = static_cast< lcsm::EvaluatorNode * >(constantWireGraph);

		// Make them to know about each other.
		constantNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(constantWireNode));
		constantWireNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(constantNode));

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

		// Power's tree node.
		lcsm::EvaluatorNode *powerNode = static_cast< lcsm::EvaluatorNode * >(powerGraph);

		// Wire's value tree node as Power's child.
		lcsm::EvaluatorNode *powerWireNode = static_cast< lcsm::EvaluatorNode * >(powerWireGraph);

		// Make them to know about each other.
		powerNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(powerWireNode));
		powerWireNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(powerNode));

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

		// Ground's tree node.
		lcsm::EvaluatorNode *groundNode = static_cast< lcsm::EvaluatorNode * >(groundGraph);

		// Wire's value tree node as Ground's child.
		lcsm::EvaluatorNode *groundWireNode = static_cast< lcsm::EvaluatorNode * >(groundWireGraph);

		// Make them to know about each other.
		groundNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(groundWireNode));
		groundWireNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(groundNode));

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
		lcsm::EvaluatorNode *transistorWireBaseNode = static_cast< lcsm::EvaluatorNode * >(transistorWireBaseGraph);

		// SrcA's Wire tree node.
		lcsm::EvaluatorNode *transistorWireSrcANode = static_cast< lcsm::EvaluatorNode * >(transistorWireSrcAGraph);

		// SrcB's Wire tree node.
		lcsm::EvaluatorNode *transistorWireSrcBNode = static_cast< lcsm::EvaluatorNode * >(transistorWireSrcBGraph);

		// Base's tree node.
		lcsm::EvaluatorNode *transistorBaseNode = static_cast< lcsm::EvaluatorNode * >(transistorBaseGraph);

		// SrcA's tree node.
		lcsm::EvaluatorNode *transistorSrcANode = static_cast< lcsm::EvaluatorNode * >(transistorSrcAGraph);

		// SrcB's tree node.
		lcsm::EvaluatorNode *transistorSrcBNode = static_cast< lcsm::EvaluatorNode * >(transistorWireSrcBGraph);

		// State's tree node.
		lcsm::EvaluatorNode *transistorStateNode = static_cast< lcsm::EvaluatorNode * >(transistorStateGraph);

		// Make them to know about each other.
		const lcsm::support::PointerView< lcsm::EvaluatorNode > transistorBaseNodeView = transistorBaseNode;
		const lcsm::support::PointerView< lcsm::EvaluatorNode > transistorSrcANodeView = transistorSrcANode;
		const lcsm::support::PointerView< lcsm::EvaluatorNode > transistorSrcBNodeView = transistorSrcBNode;

		transistorBaseNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(transistorWireBaseNode));
		transistorWireBaseNode->pushBackChild(transistorBaseNodeView);
		transistorSrcANode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(transistorWireSrcANode));
		transistorWireSrcANode->pushBackChild(transistorSrcANodeView);
		transistorSrcBNode->pushBackChild(lcsm::support::PointerView< lcsm::EvaluatorNode >(transistorWireSrcBNode));
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
*/

lcsm::support::PointerView< lcsm::EvaluatorNode > lcsm::LCSMEngine::registered(lcsm::Identifier id) const noexcept
{
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > >::const_iterator found =
		m_objects.find(id);

	if (found != m_objects.cend())
		return lcsm::support::PointerView< lcsm::EvaluatorNode >::fromSharedPtr(found->second);

	return {};
}

lcsm::support::PointerView< lcsm::EvaluatorNode > lcsm::LCSMEngine::registeredWire(lcsm::Identifier id)
{
	lcsm::support::PointerView< lcsm::EvaluatorNode > wireNode;
	const lcsm::support::PointerView< lcsm::EvaluatorNode > foundWireNode = registered(id);

	// As there is no error handling, like if found node is not wire as we expected, so we just use this private
	// pretty in right place.
	if (foundWireNode)
	{
		wireNode = foundWireNode;
	}
	else
	{
		m_objects[id] = std::make_shared< lcsm::physical::Wire >();
		wireNode = m_objects[id];
	}

	return wireNode;
}

void lcsm::LCSMEngine::buildCircuit(std::deque< lcsm::support::PointerView< const lcsm::Circuit > > &bfsVisit)
{
	std::unordered_set< lcsm::Identifier > visited;

	while (!bfsVisit.empty())
	{
		std::size_t levelSize = bfsVisit.size();

		while (levelSize != 0)
		{
			levelSize--;

			const lcsm::support::PointerView< const lcsm::Circuit > circuit = bfsVisit.front();
			bfsVisit.pop_front();

			const std::unordered_set< lcsm::Identifier >::const_iterator found = visited.find(circuit->id());
			if (found != visited.cend())
				continue;

			buildCircuit(circuit, bfsVisit, visited);
		}
	}
}

void lcsm::LCSMEngine::buildCircuit(
	const lcsm::support::PointerView< const lcsm::Circuit > &circuit,
	std::deque< lcsm::support::PointerView< const lcsm::Circuit > > &bfsVisit,
	std::unordered_set< lcsm::Identifier > &visited)
{
	switch (circuit->circuitType())
	{
	case lcsm::CircuitType::Wire:
	{
		// Extract Wire as model object.
		const lcsm::model::Wire *wire = static_cast< const lcsm::model::Wire * >(circuit.cptr());
		const std::vector< lcsm::support::PointerView< lcsm::Circuit > > &wireWires = wire->wires();
		const lcsm::support::PointerView< lcsm::Circuit > &wireConnect = wire->connect();
		const lcsm::Identifier wireId = wire->id();
		const lcsm::ObjectType wireObjectType = wire->objectType();

		// Wire's tree node.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		wireEvaluatorNode->setObjectType(wireObjectType);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Make connections from Wire object to his adjacent wires as
		// tree's edges.
		// Model guarantees, that wireWires (aka wire->wires()) is connected only to model::Wire.
		for (const lcsm::support::PointerView< lcsm::Circuit > &wireChild : wireWires)
		{
			const lcsm::Identifier wireChildId = wireChild->id();
			const lcsm::ObjectType wireChildObjectType = wireChild->objectType();
			lcsm::support::PointerView< lcsm::EvaluatorNode > wireChildEvaluatorNode = registeredWire(wireChildId);
			wireChildEvaluatorNode->setObjectType(wireChildObjectType);
			wireNode->connect(wireChildEvaluatorNode);
		}

		// This wire might be some object's wire, so we should make connection
		// as tree's edge.
		if (wireConnect)
			bfsVisit.emplace_back(wireConnect.cptr());

		// Add as visited objects.
		visited.insert(wireId);

		break;
	}
	case lcsm::CircuitType::Tunnel:
		throw std::logic_error("Building tunnel is not implemented.");
	case lcsm::CircuitType::Pin:
	{
		// Extract Pin as model object.
		const lcsm::model::Pin *pin = static_cast< const lcsm::model::Pin * >(circuit.cptr());
		const lcsm::model::Wire &wireInternal = pin->internal();
		const lcsm::model::Wire &wireExternal = pin->external();
		const lcsm::Identifier idPin = pin->id();
		const lcsm::Identifier idWireInternal = wireInternal.id();
		const lcsm::Identifier idWireExternal = wireExternal.id();
		const lcsm::ObjectType pinObjectType = pin->objectType();
		const lcsm::ObjectType wireInternalObjectType = wireInternal.objectType();
		const lcsm::ObjectType wireExternalObjectType = wireExternal.objectType();

		// Build physical Pin.
		if (pin->output())
		{
			// By the algorithm, we have never created PinOutput before this moment.
			m_objects[idPin] = std::make_shared< lcsm::physical::PinOutput >();
			lcsm::physical::PinOutput *pinOutput = static_cast< lcsm::physical::PinOutput * >(m_objects[idPin].get());
			pinOutput->setObjectType(pinObjectType);

			// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
			lcsm::support::PointerView< lcsm::EvaluatorNode > internal = registeredWire(idWireInternal);
			internal->setObjectType(wireInternalObjectType);
			lcsm::support::PointerView< lcsm::EvaluatorNode > external = registeredWire(idWireExternal);
			internal->setObjectType(wireExternalObjectType);

			// Connect wires to pin.
			pinOutput->connectInternal(internal);
			pinOutput->connectExternal(external);
		}
		else
		{
			// By the algorithm, we have never created PinInput before this moment.
			m_objects[idPin] = std::make_shared< lcsm::physical::PinInput >();
			lcsm::physical::PinInput *pinInput = static_cast< lcsm::physical::PinInput * >(m_objects[idPin].get());
			pinInput->setObjectType(pinObjectType);

			// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
			lcsm::support::PointerView< lcsm::EvaluatorNode > internal = registeredWire(idWireInternal);
			internal->setObjectType(wireInternalObjectType);
			lcsm::support::PointerView< lcsm::EvaluatorNode > external = registeredWire(idWireExternal);
			internal->setObjectType(wireExternalObjectType);

			// Connect wires to pin.
			pinInput->connectInternal(internal);
			pinInput->connectExternal(external);
		}

		// Add Pin's wire to queue.
		bfsVisit.emplace_back(std::addressof(wireExternal));
		bfsVisit.emplace_back(std::addressof(wireInternal));

		// Add as visited objects.
		visited.insert(pin->id());

		break;
	}
	case lcsm::CircuitType::Constant:
	{
		// Extract Constant as model object.
		const lcsm::model::Constant *constantModel = static_cast< const lcsm::model::Constant * >(circuit.cptr());
		const lcsm::model::Wire &wireModel = constantModel->wire();
		const lcsm::Identifier constantId = constantModel->id();
		const lcsm::Identifier wireId = wireModel.id();
		const lcsm::ObjectType constantObjectType = constantModel->objectType();
		const lcsm::ObjectType wireObjectType = wireModel.objectType();

		// By the algorithm, we have never created PinOutput before this moment.
		const lcsm::Width width = constantModel->width();
		const lcsm::value_t value = constantModel->value();
		m_objects[constantId] = std::make_shared< lcsm::physical::Constant >(lcsm::DataBits::fromModel(width, value));
		lcsm::support::PointerView< lcsm::EvaluatorNode > constantEvaluatorNode = m_objects[constantId];
		constantEvaluatorNode->setObjectType(constantObjectType);
		lcsm::support::PointerView< lcsm::physical::Constant > constantNode =
			constantEvaluatorNode.staticCast< lcsm::physical::Constant >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		wireEvaluatorNode->setObjectType(wireObjectType);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to constant. Make them to know about each other.
		constantNode->connect(wireEvaluatorNode);
		wireNode->connect(constantEvaluatorNode);

		// Add Constant's wire to queue.
		bfsVisit.emplace_back(std::addressof(wireModel));

		// Update visited objects.
		visited.insert(constantId);

		break;
	}
	case lcsm::CircuitType::Power:
	{
		// Extract Power as model object.
		const lcsm::model::Power *powerModel = static_cast< const lcsm::model::Power * >(circuit.cptr());
		const lcsm::model::Wire &wireModel = powerModel->wire();
		const lcsm::Identifier powerId = powerModel->id();
		const lcsm::Identifier wireId = wireModel.id();
		const lcsm::ObjectType powerObjectType = powerModel->objectType();
		const lcsm::ObjectType wireObjectType = wireModel.objectType();

		// By the algorithm, we have never created PinOutput before this moment.
		const lcsm::Width width = powerModel->width();
		m_objects[powerId] = std::make_shared< lcsm::physical::Power >(width);
		lcsm::support::PointerView< lcsm::EvaluatorNode > powerEvaluatorNode = m_objects[powerId];
		powerEvaluatorNode->setObjectType(powerObjectType);
		lcsm::support::PointerView< lcsm::physical::Power > powerNode = powerEvaluatorNode.staticCast< lcsm::physical::Power >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		wireEvaluatorNode->setObjectType(wireObjectType);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to power. Make them to know about each other.
		powerNode->connect(wireEvaluatorNode);
		wireNode->connect(powerEvaluatorNode);

		// Add Power's wire to queue.
		bfsVisit.emplace_back(std::addressof(wireModel));

		// Update visited objects.
		visited.insert(powerId);

		break;
	}
	case lcsm::CircuitType::Ground:
	{
		// Extract Ground as model object.
		const lcsm::model::Ground *groundModel = static_cast< const lcsm::model::Ground * >(circuit.cptr());
		const lcsm::model::Wire &wireModel = groundModel->wire();
		const lcsm::Identifier groundId = groundModel->id();
		const lcsm::Identifier wireId = wireModel.id();
		const lcsm::ObjectType groundObjectType = groundModel->objectType();
		const lcsm::ObjectType wireObjectType = wireModel.objectType();

		// By the algorithm, we have never created PinOutput before this moment.
		const lcsm::Width width = groundModel->width();
		m_objects[groundId] = std::make_shared< lcsm::physical::Ground >(width);
		lcsm::support::PointerView< lcsm::EvaluatorNode > groundEvaluatorNode = m_objects[groundId];
		groundEvaluatorNode->setObjectType(groundObjectType);
		lcsm::support::PointerView< lcsm::physical::Ground > groundNode =
			groundEvaluatorNode.staticCast< lcsm::physical::Ground >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		wireEvaluatorNode->setObjectType(wireObjectType);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to ground. Make them to know about each other.
		groundNode->connect(wireEvaluatorNode);
		wireNode->connect(groundEvaluatorNode);

		// Add Ground's wire to queue.
		bfsVisit.emplace_back(std::addressof(wireModel));

		// Update visited objects.
		visited.insert(groundId);

		break;
	}
	case lcsm::CircuitType::Clock:
		throw std::logic_error("Building clock is not implemented.");
	case lcsm::CircuitType::Transistor:
	{
		break;
	}
	case lcsm::CircuitType::TransmissionGate:
		throw std::logic_error("Building transmission gate is not implemented.");
	case lcsm::CircuitType::LastCircuitType:
		throw std::logic_error("Non-targeted circuit found!");
	default:
		throw std::logic_error("Unsupported circuit found!");
	}
}
