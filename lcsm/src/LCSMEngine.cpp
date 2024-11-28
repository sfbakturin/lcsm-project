#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Component.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/LCSMCircuit.h>
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

lcsm::LCSMEngine lcsm::LCSMEngine::fromCircuit(const lcsm::LCSMCircuit &circuit)
{
	/* Generated calculation graph. */
	lcsm::LCSMEngine engine;

	/* BFS in-future visited components. */
	std::deque< lcsm::support::PointerView< const lcsm::Component > > visit;

	/* Components. */
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Component > > &components = circuit.components();

	for (auto it = components.begin(); it != components.end(); it++)
	{
		if (!it->second->isCircuit())
			continue;

		const lcsm::CircuitComponent *circuitComponent = it->second->asCircuit();

		switch (circuitComponent->circuitComponentType())
		{
		case lcsm::CircuitComponentType::CIRCUIT_COMP_PIN:
		{
			const lcsm::model::Pin *pin = circuitComponent->asPin();

			/* If it's output, then skip it to built properly. */
			if (pin->isOutput())
				continue;

			/* Initialize input Pin node for calculation graph. */
			lcsm::CGPin *pinGraph = engine.registeredPinInput(pin->ID());
			lcsm::CGNode *pinNode = static_cast< lcsm::CGNode * >(pinGraph);
			engine.m_inputs.addRoot({ pinNode });

			/* Add as "to visit" in future building. */
			visit.emplace_back(pin);

			break;
		}
		case lcsm::CircuitComponentType::CIRCUIT_COMP_CONSTANT:
		{
			const lcsm::model::Constant *constant = circuitComponent->asConstant();

			/* Initialize Constant node for calculation graph. */
			lcsm::CGConstant *constantGraph = engine.registeredConstant(constant->ID());
			constantGraph->emplaceDataBits(constant->width(), constant->value());
			lcsm::CGNode *constantNode = static_cast< lcsm::CGNode * >(constantGraph);
			engine.m_inputs.addRoot({ constantNode });

			break;
		}
		case lcsm::CircuitComponentType::CIRCUIT_COMP_POWER:
		{
			const lcsm::model::Power *power = circuitComponent->asPower();

			/* Initialize Power node for calculation graph. */
			lcsm::CGPower *powerGraph = engine.registeredPower(power->ID());
			powerGraph->setWidth(power->width());
			lcsm::CGNode *powerNode = static_cast< lcsm::CGNode * >(powerGraph);
			engine.m_inputs.addRoot({ powerNode });

			break;
		}
		case lcsm::CircuitComponentType::CIRCUIT_COMP_GROUND:
		{
			const lcsm::model::Ground *ground = circuitComponent->asGround();

			/* Initialize Ground node for calculation graph. */
			lcsm::CGGround *groundGraph = engine.registeredGround(ground->ID());
			groundGraph->setWidth(ground->width());
			lcsm::CGNode *groundNode = static_cast< lcsm::CGNode * >(groundGraph);
			engine.m_inputs.addRoot({ groundNode });

			break;
		}
		default:
		{
			/* Do nothing, when it's not a one of above component. */
			break;
		}
		}
	}

	engine.buildCircuit(visit);

	return engine;
}

lcsm::LCSMState lcsm::LCSMEngine::fork()
{
	lcsm::LCSMState state = { m_inputs.roots(), *this };
	return state;
}

const lcsm::DataBits &lcsm::LCSMEngine::valueOf(lcsm::Identifier identifier) const
{
	const auto found = m_objects.find(identifier);

	if (found == m_objects.end())
		throw std::logic_error("Object not found");

	return found->second->read();
}

void lcsm::LCSMEngine::putValue(lcsm::Identifier identifier, const lcsm::DataBits &dataBits)
{
	const auto found = m_objects.find(identifier);

	if (found == m_objects.end())
		throw std::logic_error("Object not found");

	std::shared_ptr< lcsm::CGObject > &object = found->second;

	if (!object->isPinInput())
		throw std::logic_error("This is not a input Pin");

	lcsm::CGPinInput *pin = object->asPinInput();
	pin->externalWrite(dataBits);
}

void lcsm::LCSMEngine::putValue(lcsm::Identifier identifier, lcsm::DataBits &&dataBits)
{
	const auto found = m_objects.find(identifier);

	if (found == m_objects.end())
		throw std::logic_error("Object not found");

	std::shared_ptr< lcsm::CGObject > &object = found->second;

	if (!object->isPinInput())
		throw std::logic_error("This is not a input Pin");

	lcsm::CGPinInput *pin = object->asPinInput();
	pin->externalWrite(std::move(dataBits));
}

void lcsm::LCSMEngine::resetValues() noexcept
{
	for (auto it = m_objects.begin(); it != m_objects.end(); it++)
	{
		std::shared_ptr< lcsm::CGObject > &object = it->second;
		object->reset();
	}
}

// void lcsm::LCSMEngine::addCircuit(lcsm::LCSMCircuit &circuit)
// {
// 	if (m_circuits != 0)
// 		throw std::logic_error("");

// 	m_circuits++;

// 	std::deque< lcsm::support::PointerView< const lcsm::Component > > bfsVisit;

// 	for (auto it = circuit.Pins().begin(); it != circuit.Pins().end(); it++)
// 	{
// 		const lcsm::model::Pin *pin = it->second->asCircuit()->asPin();

// 		if (pin->isOutput())
// 			continue;

// 		// Initialize future nodes for calculation graph.
// 		lcsm::CGPin *pinGraph = registeredPinInput(pin->ID());
// 		lcsm::CGNode *pinNode = static_cast< lcsm::CGNode * >(pinGraph);
// 		m_inputs.addRoot({ pinNode });

// 		bfsVisit.emplace_back(pin);
// 	}

// 	buildCircuit(bfsVisit);
// }

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
		lcsm::CGNode *wireNode = static_cast< lcsm::CGNode * >(wireGraph);

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
				lcsm::CGNode *adjacentWireNode = static_cast< lcsm::CGNode * >(adjacentWireGraph);

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
		lcsm::CGNode *pinNode = static_cast< lcsm::CGNode * >(pinGraph);

		// Wire's tree node as Pin's child.
		lcsm::CGNode *pinWireNode = static_cast< lcsm::CGNode * >(pinWireGraph);

		// Make them to know about each other.
		pinNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(pinWireNode));
		pinWireNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(pinNode));

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
		lcsm::CGNode *constantNode = static_cast< lcsm::CGNode * >(constantGraph);

		// Wire's tree node as Constant's child.
		lcsm::CGNode *constantWireNode = static_cast< lcsm::CGNode * >(constantWireGraph);

		// Make them to know about each other.
		constantNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(constantWireNode));
		constantWireNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(constantNode));

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
		lcsm::CGNode *powerNode = static_cast< lcsm::CGNode * >(powerGraph);

		// Wire's value tree node as Power's child.
		lcsm::CGNode *powerWireNode = static_cast< lcsm::CGNode * >(powerWireGraph);

		// Make them to know about each other.
		powerNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(powerWireNode));
		powerWireNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(powerNode));

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
		lcsm::CGNode *groundNode = static_cast< lcsm::CGNode * >(groundGraph);

		// Wire's value tree node as Ground's child.
		lcsm::CGNode *groundWireNode = static_cast< lcsm::CGNode * >(groundWireGraph);

		// Make them to know about each other.
		groundNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(groundWireNode));
		groundWireNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(groundNode));

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
		lcsm::CGNode *transistorWireBaseNode = static_cast< lcsm::CGNode * >(transistorWireBaseGraph);

		// SrcA's Wire tree node.
		lcsm::CGNode *transistorWireSrcANode = static_cast< lcsm::CGNode * >(transistorWireSrcAGraph);

		// SrcB's Wire tree node.
		lcsm::CGNode *transistorWireSrcBNode = static_cast< lcsm::CGNode * >(transistorWireSrcBGraph);

		// Base's tree node.
		lcsm::CGNode *transistorBaseNode = static_cast< lcsm::CGNode * >(transistorBaseGraph);

		// SrcA's tree node.
		lcsm::CGNode *transistorSrcANode = static_cast< lcsm::CGNode * >(transistorSrcAGraph);

		// SrcB's tree node.
		lcsm::CGNode *transistorSrcBNode = static_cast< lcsm::CGNode * >(transistorWireSrcBGraph);

		// State's tree node.
		lcsm::CGNode *transistorStateNode = static_cast< lcsm::CGNode * >(transistorStateGraph);

		// Make them to know about each other.
		const lcsm::support::PointerView< lcsm::CGNode > transistorBaseNodeView = transistorBaseNode;
		const lcsm::support::PointerView< lcsm::CGNode > transistorSrcANodeView = transistorSrcANode;
		const lcsm::support::PointerView< lcsm::CGNode > transistorSrcBNodeView = transistorSrcBNode;

		transistorBaseNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(transistorWireBaseNode));
		transistorWireBaseNode->pushBackChild(transistorBaseNodeView);
		transistorSrcANode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(transistorWireSrcANode));
		transistorWireSrcANode->pushBackChild(transistorSrcANodeView);
		transistorSrcBNode->pushBackChild(lcsm::support::PointerView< lcsm::CGNode >(transistorWireSrcBNode));
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
