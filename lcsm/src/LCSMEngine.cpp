#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/std/Clock.h>
#include <lcsm/Physical/std/Constant.h>
#include <lcsm/Physical/std/Ground.h>
#include <lcsm/Physical/std/Pin.h>
#include <lcsm/Physical/std/Power.h>
#include <lcsm/Physical/std/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>
#include <unordered_set>

#include <deque>
#include <memory>
#include <stdexcept>
#include <vector>

lcsm::LCSMEngine lcsm::LCSMEngine::fromCircuit(const lcsm::LCSMCircuit &circuit)
{
	// Generated calculation graph.
	lcsm::LCSMEngine engine;

	// BFS in-future visited components.
	std::deque< lcsm::support::PointerView< const lcsm::Circuit > > visit;

	// Components.
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &inputs = circuit.inputs();
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &outputs = circuit.outputs();

	// Find all *In*/*Out* circuits and add them to BFS queue.
	for (auto it = inputs.begin(); it != inputs.end(); it++)
	{
		lcsm::support::PointerView< lcsm::Circuit > circ = lcsm::support::PointerView< lcsm::Circuit >::fromSharedPtr(it->second);
		visit.emplace_back(circ.get());
	}

	for (auto it = outputs.begin(); it != outputs.end(); it++)
	{
		lcsm::support::PointerView< lcsm::Circuit > circ = lcsm::support::PointerView< lcsm::Circuit >::fromSharedPtr(it->second);
		visit.emplace_back(circ.get());
	}

	engine.buildCircuit(visit);
	engine.m_circuit = std::addressof(circuit);
	return engine;
}

lcsm::LCSMState lcsm::LCSMEngine::fork()
{
	lcsm::LCSMState state = this;
	return state;
}

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
		// Hardcode...
		m_objects[id] = std::make_shared< lcsm::physical::Wire >(lcsm::ObjectType::Wiring | lcsm::ObjectType::Internal);
		wireNode = m_objects[id];
	}

	return wireNode;
}

void lcsm::LCSMEngine::buildCircuit(std::deque< lcsm::support::PointerView< const lcsm::Circuit > > &bfsVisit)
{
	std::unordered_set< lcsm::Identifier > visited;

	while (!bfsVisit.empty())
	{
		const lcsm::support::PointerView< const lcsm::Circuit > circuit = bfsVisit.front();
		bfsVisit.pop_front();

		const std::unordered_set< lcsm::Identifier >::const_iterator found = visited.find(circuit->id());
		if (found != visited.cend())
			continue;

		buildCircuit(circuit, bfsVisit, visited);
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
		const lcsm::model::Wire *wire = static_cast< const lcsm::model::Wire * >(circuit.get());
		const std::vector< lcsm::support::PointerView< lcsm::Circuit > > &wireWires = wire->wires();
		const lcsm::support::PointerView< lcsm::Circuit > &wireConnect = wire->connect();
		const lcsm::Identifier wireId = wire->id();

		// Wire's tree node.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Make connections from Wire object to his adjacent wires as
		// tree's edges.
		// Model guarantees, that wireWires (aka wire->wires()) is connected only to model::Wire.
		for (const lcsm::support::PointerView< lcsm::Circuit > &wireChild : wireWires)
		{
			const lcsm::Identifier wireChildId = wireChild->id();
			lcsm::support::PointerView< lcsm::EvaluatorNode > wireChildEvaluatorNode = registeredWire(wireChildId);
			wireNode->connect(wireChildEvaluatorNode);
			bfsVisit.emplace_back(wireChild.get());
		}

		// This wire might be some object's wire, so we should make connection
		// as tree's edge.
		if (wireConnect)
			bfsVisit.emplace_back(wireConnect.get());

		// Add as visited objects.
		visited.insert(wireId);

		break;
	}
	case lcsm::CircuitType::Tunnel:
		throw std::logic_error("Building tunnel is not implemented.");
	case lcsm::CircuitType::Pin:
	{
		// Extract Pin as model object.
		const lcsm::model::Pin *pin = static_cast< const lcsm::model::Pin * >(circuit.get());
		const lcsm::model::Wire *wireInternal = pin->internal();
		const lcsm::model::Wire *wireExternal = pin->external();
		const lcsm::Identifier idPin = pin->id();
		const lcsm::Identifier idWireInternal = wireInternal->id();
		const lcsm::Identifier idWireExternal = wireExternal->id();
		const lcsm::object_type_t pinObjectType = pin->objectType();

		// By the algorithm, we have never created Pin before this moment.
		m_objects[idPin] = std::make_shared< lcsm::physical::Pin >(pinObjectType, pin->output());
		lcsm::physical::Pin *pinPhysical = static_cast< lcsm::physical::Pin * >(m_objects[idPin].get());

		// 	// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > internal = registeredWire(idWireInternal);
		lcsm::support::PointerView< lcsm::physical::Wire > internalWire = internal.staticCast< lcsm::physical::Wire >();
		lcsm::support::PointerView< lcsm::EvaluatorNode > external = registeredWire(idWireExternal);
		lcsm::support::PointerView< lcsm::physical::Wire > externalWire = external.staticCast< lcsm::physical::Wire >();

		// Connect wires to pin.
		pinPhysical->connectInternal(internal);
		internalWire->connect(m_objects[idPin]);
		pinPhysical->connectExternal(external);
		externalWire->connect(m_objects[idPin]);

		// Add Pin's wire to queue.
		bfsVisit.emplace_back(wireInternal);
		bfsVisit.emplace_back(wireExternal);

		// Add as visited objects.
		visited.insert(pin->id());

		break;
	}
	case lcsm::CircuitType::Constant:
	{
		// Extract Constant as model object.
		const lcsm::model::Constant *constantModel = static_cast< const lcsm::model::Constant * >(circuit.get());
		const lcsm::model::Wire *wireModel = constantModel->wire();
		const lcsm::Identifier constantId = constantModel->id();
		const lcsm::Identifier wireId = wireModel->id();
		const lcsm::object_type_t constantObjectType = constantModel->objectType();

		// By the algorithm, we have never created Constant before this moment.
		const lcsm::Width width = constantModel->width();
		const lcsm::value_t value = constantModel->value();
		m_objects[constantId] =
			std::make_shared< lcsm::physical::Constant >(constantObjectType, lcsm::DataBits::fromModel(width, value));
		lcsm::support::PointerView< lcsm::EvaluatorNode > constantEvaluatorNode = m_objects[constantId];
		lcsm::support::PointerView< lcsm::physical::Constant > constantNode =
			constantEvaluatorNode.staticCast< lcsm::physical::Constant >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to constant. Make them to know about each other.
		constantNode->connect(wireEvaluatorNode);
		wireNode->connect(constantEvaluatorNode);

		// Add Constant's wire to queue.
		bfsVisit.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(constantId);

		break;
	}
	case lcsm::CircuitType::Power:
	{
		// Extract Power as model object.
		const lcsm::model::Power *powerModel = static_cast< const lcsm::model::Power * >(circuit.get());
		const lcsm::model::Wire *wireModel = powerModel->wire();
		const lcsm::Identifier powerId = powerModel->id();
		const lcsm::Identifier wireId = wireModel->id();
		const lcsm::object_type_t powerObjectType = powerModel->objectType();

		// By the algorithm, we have never created Power before this moment.
		const lcsm::Width width = powerModel->width();
		m_objects[powerId] = std::make_shared< lcsm::physical::Power >(powerObjectType, width);
		lcsm::support::PointerView< lcsm::EvaluatorNode > powerEvaluatorNode = m_objects[powerId];
		lcsm::support::PointerView< lcsm::physical::Power > powerNode = powerEvaluatorNode.staticCast< lcsm::physical::Power >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to power. Make them to know about each other.
		powerNode->connect(wireEvaluatorNode);
		wireNode->connect(powerEvaluatorNode);

		// Add Power's wire to queue.
		bfsVisit.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(powerId);

		break;
	}
	case lcsm::CircuitType::Ground:
	{
		// Extract Ground as model object.
		const lcsm::model::Ground *groundModel = static_cast< const lcsm::model::Ground * >(circuit.get());
		const lcsm::model::Wire *wireModel = groundModel->wire();
		const lcsm::Identifier groundId = groundModel->id();
		const lcsm::Identifier wireId = wireModel->id();
		const lcsm::object_type_t groundObjectType = groundModel->objectType();

		// By the algorithm, we have never created PinOutput before this moment.
		const lcsm::Width width = groundModel->width();
		m_objects[groundId] = std::make_shared< lcsm::physical::Ground >(groundObjectType, width);
		lcsm::support::PointerView< lcsm::EvaluatorNode > groundEvaluatorNode = m_objects[groundId];
		lcsm::support::PointerView< lcsm::physical::Ground > groundNode =
			groundEvaluatorNode.staticCast< lcsm::physical::Ground >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to ground. Make them to know about each other.
		groundNode->connect(wireEvaluatorNode);
		wireNode->connect(groundEvaluatorNode);

		// Add Ground's wire to queue.
		bfsVisit.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(groundId);

		break;
	}
	case lcsm::CircuitType::Clock:
	{
		// Extract Clock as model object.
		const lcsm::model::Clock *clockModel = static_cast< const lcsm::model::Clock * >(circuit.get());
		const lcsm::model::Wire *wireModel = clockModel->wire();
		const lcsm::Identifier clockId = clockModel->id();
		const lcsm::Identifier wireId = wireModel->id();
		const lcsm::object_type_t clockObjectType = clockModel->objectType();

		// By the algorithm, we have never created Clock before this moment.
		const unsigned highDuration = clockModel->highDuration();
		const unsigned lowDuration = clockModel->lowDuration();
		const unsigned phaseOffset = clockModel->phaseOffset();
		m_objects[clockId] = std::make_shared< lcsm::physical::Clock >(clockObjectType, highDuration, lowDuration, phaseOffset);
		lcsm::support::PointerView< lcsm::EvaluatorNode > clockEvaluatorNode = m_objects[clockId];
		lcsm::support::PointerView< lcsm::physical::Clock > clockNode = clockEvaluatorNode.staticCast< lcsm::physical::Clock >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to constant. Make them to know about each other.
		clockNode->connect(wireEvaluatorNode);
		wireNode->connect(clockEvaluatorNode);

		// Add Constant's wire to queue.
		bfsVisit.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(clockId);

		break;
	}
	case lcsm::CircuitType::Transistor:
		break;
	case lcsm::CircuitType::TransmissionGate:
		throw std::logic_error("Building transmission gate is not implemented.");
	case lcsm::CircuitType::LastCircuitType:
		throw std::logic_error("Non-targeted circuit found!");
	default:
		throw std::logic_error("Unsupported circuit found!");
	}
}
