#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Verilog.h>
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
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/std/Button.h>
#include <lcsm/Physical/std/Clock.h>
#include <lcsm/Physical/std/Constant.h>
#include <lcsm/Physical/std/Digit.h>
#include <lcsm/Physical/std/Ground.h>
#include <lcsm/Physical/std/Pin.h>
#include <lcsm/Physical/std/Power.h>
#include <lcsm/Physical/std/Probe.h>
#include <lcsm/Physical/std/Splitter.h>
#include <lcsm/Physical/std/Transistor.h>
#include <lcsm/Physical/std/TransmissionGate.h>
#include <lcsm/Physical/std/Tunnel.h>
#include <lcsm/Physical/std/Verilog.h>
#include <lcsm/Physical/std/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>
#include <unordered_map>
#include <unordered_set>

#include <deque>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::LCSMEngine lcsm::LCSMEngine::fromCircuit(const lcsm::LCSMCircuit &circuit)
{
	// Generated calculation graph.
	lcsm::LCSMEngine engine;

	// BFS in-future visited components.
	std::deque< lcsm::support::PointerView< const lcsm::Component > > queue;

	// Components.
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Component > > &components = circuit.components();

	// Find all *In*/*Out* circuits and add them to BFS queue.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		lcsm::Component *circ = it->second.get();
		if (lcsm::TestObjectType(circ->objectType(), lcsm::ObjectType::Root))
		{
			queue.emplace_back(circ);
		}
	}

	// Build it!
	engine.buildCircuit(queue);

	// Components helpers.
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Component > > &inputs = circuit.inputs();
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Component > > &outputs = circuit.outputs();

	// Find all *In*/*Out*/*Root* circuit and add them to helpers.
	for (auto it = engine.m_objects.begin(); it != engine.m_objects.end(); it++)
	{
		const lcsm::Identifier id = it->first;
		// Add to real inputs, if id is presented in inputs.
		if (inputs.find(id) != inputs.end())
		{
			engine.m_realInputs[id] = engine.m_objects[id];
			engine.m_realInputsIds.push_back(id);
		}
		else	// Add to real outputs, if id is presented in outputs.
			if (outputs.find(id) != outputs.end())
			{
				engine.m_realOutputs[id] = engine.m_objects[id];
				engine.m_realOutputsIds.push_back(id);
			}
			else
			{
				// Add to real roots, if id is *pure* root.
				const lcsm::object_type_t objectType = it->second->objectType();
				if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Root) && !lcsm::TestObjectType(objectType, lcsm::ObjectType::Input))
				{
					engine.m_realRoots[id] = engine.m_objects[id];
					engine.m_realOutputsIds.push_back(id);
				}
			}
	}

	return engine;
}

lcsm::LCSMState lcsm::LCSMEngine::fork()
{
	lcsm::LCSMState state = this;
	return state;
}

const std::vector< lcsm::Identifier > &lcsm::LCSMEngine::getInputIndexes() const noexcept
{
	return m_realInputsIds;
}

const std::vector< lcsm::Identifier > &lcsm::LCSMEngine::getOutputIndexes() const noexcept
{
	return m_realOutputsIds;
}

const std::vector< lcsm::Identifier > &lcsm::LCSMEngine::getRootIndexes() const noexcept
{
	return m_realRootsIds;
}

lcsm::support::PointerView< lcsm::EvaluatorNode > lcsm::LCSMEngine::registered(lcsm::Identifier id) const noexcept
{
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::EvaluatorNode > >::const_iterator found =
		m_objects.find(id);

	if (found != m_objects.cend())
		return lcsm::support::PointerView< lcsm::EvaluatorNode >::fromSharedPtr(found->second);

	return {};
}

// Hardcoded value of object type for lcsm::LCSMEngine::registered(Wire|Tunnel) methods.
// FIXME: In future this must die.
static constexpr lcsm::object_type_t WiringObject = lcsm::ObjectType::Internal | lcsm::ObjectType::Wiring;

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
		m_objects[id] = std::make_shared< lcsm::physical::Wire >(WiringObject);
		wireNode = m_objects[id];
	}

	return wireNode;
}

lcsm::support::PointerView< lcsm::EvaluatorNode > lcsm::LCSMEngine::registeredTunnel(lcsm::Identifier id)
{
	lcsm::support::PointerView< lcsm::EvaluatorNode > tunnelNode;
	const lcsm::support::PointerView< lcsm::EvaluatorNode > foundTunnelNode = registered(id);

	// As there is no error handling, like if found node is not wire as we expected, so we just use this private
	// pretty in right place.
	if (foundTunnelNode)
	{
		tunnelNode = foundTunnelNode;
	}
	else
	{
		m_objects[id] = std::make_shared< lcsm::physical::Tunnel >(WiringObject);
		tunnelNode = m_objects[id];
	}

	return tunnelNode;
}

void lcsm::LCSMEngine::buildCircuit(std::deque< lcsm::support::PointerView< const lcsm::Component > > &queue)
{
	std::unordered_set< lcsm::Identifier > visited;

	while (!queue.empty())
	{
		const lcsm::support::PointerView< const lcsm::Component > circuit = queue.front();
		queue.pop_front();

		const std::unordered_set< lcsm::Identifier >::const_iterator found = visited.find(circuit->id());
		if (found != visited.cend())
			continue;

		buildCircuit(circuit, queue, visited);
	}
}

void lcsm::LCSMEngine::buildCircuit(
	const lcsm::support::PointerView< const lcsm::Component > &circuit,
	std::deque< lcsm::support::PointerView< const lcsm::Component > > &queue,
	std::unordered_set< lcsm::Identifier > &visited)
{
	switch (circuit->componentType())
	{
	case lcsm::ComponentType::Wire:
	{
		// Extract Wire as model object.
		const lcsm::model::Wire *wire = static_cast< const lcsm::model::Wire * >(circuit.get());
		const std::vector< lcsm::support::PointerView< lcsm::Component > > &wireWires = wire->wires();
		const lcsm::support::PointerView< lcsm::Component > &wireConnect = wire->connect();
		const lcsm::Identifier wireId = wire->id();

		// Wire's tree node.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Make connections from Wire object to his adjacent wires as
		// tree's edges.
		// Model guarantees, that wireWires (aka wire->wires()) is connected only to model::Wire.
		for (const lcsm::support::PointerView< lcsm::Component > &wireChild : wireWires)
		{
			const lcsm::Identifier wireChildId = wireChild->id();
			lcsm::support::PointerView< lcsm::EvaluatorNode > wireChildEvaluatorNode = registeredWire(wireChildId);
			wireNode->connect(wireChildEvaluatorNode);
			queue.emplace_back(wireChild.get());
		}

		// This wire might be some object's wire, so we should make connection
		// as tree's edge.
		if (wireConnect)
			queue.emplace_back(wireConnect.get());

		// Add as visited objects.
		visited.insert(wireId);

		break;
	}
	case lcsm::ComponentType::Tunnel:
	{
		// Extract Tunnel as model object.
		const lcsm::model::Tunnel *tunnel = static_cast< const lcsm::model::Tunnel * >(circuit.get());
		const lcsm::model::Wire *tunnelWire = tunnel->wire();
		const std::vector< lcsm::Component * > &tunnelTunnels = tunnel->tunnels();
		const lcsm::Identifier tunnelId = tunnel->id();
		const lcsm::Identifier tunnelWireId = tunnelWire->id();

		// Tunnel's tree node.
		lcsm::support::PointerView< lcsm::EvaluatorNode > tunnelEvaluatorNode = registeredTunnel(tunnelId);
		lcsm::support::PointerView< lcsm::physical::Tunnel > tunnelNode =
			tunnelEvaluatorNode.staticCast< lcsm::physical::Tunnel >();

		// Make connections from Tunnel object to his adjacent tunnels as tree's edges. Model guarantees, that
		// tunnelTunnels (aka tunnel->tunnels()) is connect only to model::Tunnel.
		for (const lcsm::Component *tunnelChild : tunnelTunnels)
		{
			const lcsm::Identifier tunnelChildId = tunnelChild->id();
			lcsm::support::PointerView< lcsm::EvaluatorNode > tunnelChildEvaluatorNode = registeredTunnel(tunnelChildId);
			tunnelNode->connectTunnel(tunnelChildEvaluatorNode);
			queue.emplace_back(tunnelChild);
		}

		// Make connect to tunnel's wiring as tree's node.
		lcsm::support::PointerView< lcsm::EvaluatorNode > tunnelWireEvaluatorNode = registeredWire(tunnelWireId);
		lcsm::support::PointerView< lcsm::physical::Wire > tunnelWireNode =
			tunnelWireEvaluatorNode.staticCast< lcsm::physical::Wire >();
		tunnelNode->connectWiring(tunnelWireEvaluatorNode);
		tunnelWireNode->connect(tunnelEvaluatorNode);
		queue.emplace_back(tunnelWire);

		// Add as visited objects.
		visited.insert(tunnelId);

		break;
	}
	case lcsm::ComponentType::Pin:
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
		m_objects[idPin] = std::make_shared< lcsm::physical::Pin >(pinObjectType, pin->output(), pin->width());
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
		queue.emplace_back(wireInternal);
		queue.emplace_back(wireExternal);

		// Add as visited objects.
		visited.insert(pin->id());

		break;
	}
	case lcsm::ComponentType::Constant:
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
		queue.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(constantId);

		break;
	}
	case lcsm::ComponentType::Power:
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
		queue.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(powerId);

		break;
	}
	case lcsm::ComponentType::Ground:
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
		queue.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(groundId);

		break;
	}
	case lcsm::ComponentType::Clock:
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
		queue.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(clockId);

		break;
	}
	case lcsm::ComponentType::Transistor:
	{
		// Extract Transistor as model object.
		const lcsm::model::Transistor *transistorModel = static_cast< const lcsm::model::Transistor * >(circuit.get());
		const lcsm::model::Wire *transistorBaseModel = transistorModel->wireBase();
		const lcsm::model::Wire *transistorSrcAModel = transistorModel->wireSrcA();
		const lcsm::model::Wire *transistorSrcBModel = transistorModel->wireSrcB();
		const lcsm::Identifier transistorId = transistorModel->id();
		const lcsm::Identifier baseId = transistorBaseModel->id();
		const lcsm::Identifier srcAId = transistorSrcAModel->id();
		const lcsm::Identifier srcBId = transistorSrcBModel->id();

		// By the algorithm, we have never create Transistor physical object before this moment.
		const lcsm::model::Transistor::Type type = transistorModel->type();
		const lcsm::object_type_t objectType = transistorModel->objectType();
		std::shared_ptr< lcsm::EvaluatorNode > transistorPhysical = std::make_shared< lcsm::physical::Transistor >(objectType, type);
		m_objects[transistorId] = transistorPhysical;
		lcsm::support::PointerView< lcsm::EvaluatorNode > transistorEvaluatorNode = transistorPhysical;
		lcsm::support::PointerView< lcsm::physical::Transistor > transistorNode =
			transistorEvaluatorNode.staticCast< lcsm::physical::Transistor >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireBaseEvaluatorNode = registeredWire(baseId);
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireSrcAEvaluatorNode = registeredWire(srcAId);
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireSrcBEvaluatorNode = registeredWire(srcBId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireBaseNode =
			wireBaseEvaluatorNode.staticCast< lcsm::physical::Wire >();
		lcsm::support::PointerView< lcsm::physical::Wire > wireSrcANode =
			wireSrcAEvaluatorNode.staticCast< lcsm::physical::Wire >();
		lcsm::support::PointerView< lcsm::physical::Wire > wireSrcBNode =
			wireSrcBEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wires to transistor. Make them to know about each other.
		transistorNode->connectBase(wireBaseEvaluatorNode);
		transistorNode->connectSrcA(wireSrcAEvaluatorNode);
		transistorNode->connectSrcB(wireSrcBEvaluatorNode);
		wireBaseNode->connect(transistorEvaluatorNode);
		wireSrcANode->connect(transistorEvaluatorNode);
		wireSrcBNode->connect(transistorEvaluatorNode);

		// Add wires to queue.
		queue.emplace_back(transistorBaseModel);
		queue.emplace_back(transistorSrcAModel);
		queue.emplace_back(transistorSrcBModel);

		// Update visited objects.
		visited.insert(transistorId);

		break;
	}
	case lcsm::ComponentType::TransmissionGate:
	{
		// Extract TransmissionGate as model object.
		const lcsm::model::TransmissionGate *transmissionGateModel = circuit.getCast< lcsm::model::TransmissionGate >();
		const lcsm::model::Wire *wireBaseModel = transmissionGateModel->wireBase();
		const lcsm::model::Wire *wireSrcAModel = transmissionGateModel->wireSrcA();
		const lcsm::model::Wire *wireSrcBModel = transmissionGateModel->wireSrcB();
		const lcsm::model::Wire *wireSrcCModel = transmissionGateModel->wireSrcC();
		const lcsm::Identifier id = transmissionGateModel->id();
		const lcsm::Identifier baseId = wireBaseModel->id();
		const lcsm::Identifier srcAId = wireSrcAModel->id();
		const lcsm::Identifier srcBId = wireSrcBModel->id();
		const lcsm::Identifier srcCId = wireSrcCModel->id();

		// By the algorithm, we have never create TransmissionGate physical object before this moment.
		const lcsm::object_type_t objectType = transmissionGateModel->objectType();
		std::shared_ptr< lcsm::EvaluatorNode > transmissionGatePhysical =
			std::make_shared< lcsm::physical::TransmissionGate >(objectType);
		m_objects[id] = transmissionGatePhysical;
		lcsm::support::PointerView< lcsm::EvaluatorNode > transmissionGateEvaluatorNode = transmissionGatePhysical;
		lcsm::support::PointerView< lcsm::physical::TransmissionGate > transmissionGateNode =
			transmissionGateEvaluatorNode.staticCast< lcsm::physical::TransmissionGate >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireBaseEvaluatorNode = registeredWire(baseId);
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireSrcAEvaluatorNode = registeredWire(srcAId);
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireSrcBEvaluatorNode = registeredWire(srcBId);
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireSrcCEvaluatorNode = registeredWire(srcCId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireBaseNode =
			wireBaseEvaluatorNode.staticCast< lcsm::physical::Wire >();
		lcsm::support::PointerView< lcsm::physical::Wire > wireSrcANode =
			wireSrcAEvaluatorNode.staticCast< lcsm::physical::Wire >();
		lcsm::support::PointerView< lcsm::physical::Wire > wireSrcBNode =
			wireSrcBEvaluatorNode.staticCast< lcsm::physical::Wire >();
		lcsm::support::PointerView< lcsm::physical::Wire > wireSrcCNode =
			wireSrcCEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wires to transistor. Make them to know about each other.
		transmissionGateNode->connectBase(wireBaseEvaluatorNode);
		transmissionGateNode->connectSrcA(wireSrcAEvaluatorNode);
		transmissionGateNode->connectSrcB(wireSrcBEvaluatorNode);
		transmissionGateNode->connectSrcC(wireSrcCEvaluatorNode);
		wireBaseNode->connect(transmissionGateEvaluatorNode);
		wireSrcANode->connect(transmissionGateEvaluatorNode);
		wireSrcBNode->connect(transmissionGateEvaluatorNode);
		wireSrcCNode->connect(transmissionGateEvaluatorNode);

		// Add wires to queue.
		queue.emplace_back(wireBaseModel);
		queue.emplace_back(wireSrcAModel);
		queue.emplace_back(wireSrcBModel);
		queue.emplace_back(wireSrcCModel);

		// Update visited objects.
		visited.insert(id);

		break;
	}
	case lcsm::ComponentType::Button:
	{
		// Extract Button as model object.
		const lcsm::model::Button *buttonModel = static_cast< const lcsm::model::Button * >(circuit.get());
		const lcsm::model::Wire *wireModel = buttonModel->wire();
		const lcsm::Identifier buttonId = buttonModel->id();
		const lcsm::Identifier wireId = wireModel->id();

		// By the algorithm, we have never create Button physical object before this moment.
		const bool activeOnPress = buttonModel->activeOnPress();
		const lcsm::object_type_t objectType = buttonModel->objectType();
		std::shared_ptr< lcsm::EvaluatorNode > buttonPhysical = std::make_shared< lcsm::physical::Button >(objectType, activeOnPress);
		m_objects[buttonId] = buttonPhysical;
		lcsm::support::PointerView< lcsm::EvaluatorNode > buttonEvaluatorNode = buttonPhysical;
		lcsm::support::PointerView< lcsm::physical::Button > buttonNode =
			buttonEvaluatorNode.staticCast< lcsm::physical::Button >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to button. Make them to know about each other.
		buttonNode->connect(wireEvaluatorNode);
		wireNode->connect(buttonEvaluatorNode);

		// Add Button's wire to queue.
		queue.emplace_back(wireModel);

		// Update visited objects.
		visited.insert(buttonId);

		break;
	}
	case lcsm::ComponentType::Digit:
	{
		// Extract Digit as model object.
		const lcsm::model::Digit *digitModel = static_cast< const lcsm::model::Digit * >(circuit.get());
		const lcsm::model::Wire *wireDataModel = digitModel->wireData();
		const lcsm::model::Wire *wireDecimalPointModel = digitModel->wireDecimalPoint();
		const lcsm::Identifier digitId = digitModel->id();
		const lcsm::Identifier wireDataId = wireDataModel->id();
		const lcsm::Identifier wireDecimalPointId = wireDecimalPointModel->id();
		const lcsm::object_type_t digitObjectType = digitModel->objectType();

		// By the algorithm, we have never create Digit physical object before this moment.
		const bool hasDecimalPoint = digitModel->hasDecimalPoint();
		std::shared_ptr< lcsm::EvaluatorNode > digitPhysical =
			std::make_shared< lcsm::physical::Digit >(digitObjectType, hasDecimalPoint);
		m_objects[digitId] = digitPhysical;
		lcsm::support::PointerView< lcsm::EvaluatorNode > digitEvaluatorNode = digitPhysical;
		lcsm::support::PointerView< lcsm::physical::Digit > digitNode = digitEvaluatorNode.staticCast< lcsm::physical::Digit >();

		// Ensure existence of wires: they might be already in collection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireDataEvaluatorNode = registeredWire(wireDataId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireDataNode =
			wireDataEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to ground. Make them to know about each other.
		digitNode->connectToData(wireDataEvaluatorNode);
		wireDataNode->connect(digitEvaluatorNode);

		// Add Ground's wire to queue.
		queue.emplace_back(wireDataModel);

		// Ensure existence of wire for decimal point: they might be already in collection.
		if (hasDecimalPoint)
		{
			lcsm::support::PointerView< lcsm::EvaluatorNode > wireDecimalPointEvaluatorNode = registeredWire(wireDecimalPointId);
			lcsm::support::PointerView< lcsm::physical::Wire > wireDecimalPointNode =
				wireDecimalPointEvaluatorNode.staticCast< lcsm::physical::Wire >();
			digitNode->connectToDecimalPoint(wireDecimalPointEvaluatorNode);
			wireDecimalPointNode->connect(digitEvaluatorNode);
			queue.emplace_back(wireDecimalPointModel);
		}

		// Update visited objects.
		visited.insert(digitId);

		break;
	}
	case lcsm::ComponentType::Probe:
	{
		// Extract Probe as model object.
		const lcsm::model::Probe *probeModel = static_cast< const lcsm::model::Probe * >(circuit.get());
		const lcsm::model::Wire *probeWireModel = probeModel->wire();
		const lcsm::Identifier probeId = probeModel->id();
		const lcsm::Identifier probeWireId = probeWireModel->id();
		const lcsm::object_type_t probeObjectType = probeModel->objectType();

		// By the algorithm, we have never create Probe physical object before this moment.
		std::shared_ptr< lcsm::EvaluatorNode > probePhysical = std::make_shared< lcsm::physical::Probe >(probeObjectType);
		m_objects[probeId] = probePhysical;
		lcsm::support::PointerView< lcsm::EvaluatorNode > probeEvaluatorNode = probePhysical;
		lcsm::support::PointerView< lcsm::physical::Probe > probeNode = probeEvaluatorNode.staticCast< lcsm::physical::Probe >();

		// Ensure existence of wires: they might be already in colection, so we shouldn't recreate them.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(probeWireId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to Probe element. Make them to know about each other.
		probeNode->connect(wireEvaluatorNode);
		wireNode->connect(probeEvaluatorNode);

		// Add Probe's wire to queue.
		queue.emplace_back(probeWireModel);

		// Update visited objects.
		visited.insert(probeId);

		break;
	}
	case lcsm::ComponentType::Splitter:
	{
		// Extract Splitter as model object.
		const lcsm::model::Splitter *splitterModel = circuit.getCast< lcsm::model::Splitter >();
		const lcsm::model::Wire *splitterWireInModel = splitterModel->wireIn();
		const lcsm::Identifier splitterId = splitterModel->id();
		const lcsm::Identifier splitterWireInId = splitterWireInModel->id();

		// By the algorithm, we have never created Splitter before this moment.
		const lcsm::object_type_t objectType = splitterModel->objectType();
		std::shared_ptr< lcsm::EvaluatorNode > splitterPhysical = std::make_shared< lcsm::physical::Splitter >(objectType);
		m_objects[splitterId] = splitterPhysical;
		lcsm::support::PointerView< lcsm::EvaluatorNode > splitterEvaluatorNode = splitterPhysical;
		lcsm::support::PointerView< lcsm::physical::Splitter > splitterNode =
			splitterEvaluatorNode.staticCast< lcsm::physical::Splitter >();

		// Ensure existence of input wire.
		lcsm::support::PointerView< lcsm::EvaluatorNode > wireInEvaluatorNode = registeredWire(splitterWireInId);
		lcsm::support::PointerView< lcsm::physical::Wire > wireInNode = wireInEvaluatorNode.staticCast< lcsm::physical::Wire >();

		// Connect wire to input. Make them to know about each other.
		splitterNode->connectInput(wireInEvaluatorNode);
		wireInNode->connect(splitterEvaluatorNode);

		// Add Splitter's wire to queue.
		queue.emplace_back(splitterWireInModel);

		// Update visited objects.
		visited.insert(splitterId);

		// Ensure existence of outputs. Connect to output and make them to know about each other.
		const lcsm::width_t widthOut = splitterModel->widthOut();
		for (lcsm::width_t portId = 0; portId < widthOut; portId++)
		{
			// Extract model.
			const lcsm::model::Wire *splitterWireOutModel = splitterModel->wireOut(portId);
			const lcsm::Identifier wireOutId = splitterWireOutModel->id();
			const std::pair< lcsm::width_t, lcsm::width_t > &index = splitterModel->bitsOut(portId);

			// Ensure existing wire.
			lcsm::support::PointerView< lcsm::EvaluatorNode > wireOutEvaluatorNode = registeredWire(wireOutId);
			lcsm::support::PointerView< lcsm::physical::Wire > wireOutNode =
				wireOutEvaluatorNode.staticCast< lcsm::physical::Wire >();

			// Connect to each other.
			splitterNode->connectOut(wireOutEvaluatorNode, index);
			wireOutNode->connect(splitterEvaluatorNode);

			// Add wire to visit.
			queue.emplace_back(splitterWireOutModel);
		}

		break;
	}
	case lcsm::ComponentType::VerilogModule:
	{
		// Extract VerilogModule as model object.
		const lcsm::model::VerilogModule *verilogModuleModel = circuit.getCast< lcsm::model::VerilogModule >();
		const lcsm::Identifier verilogModuleId = verilogModuleModel->id();

		// By the algorithm, we have never created VerilogModule before this moment.
		const lcsm::object_type_t objectType = verilogModuleModel->objectType();
		const lcsm::verilog::Module *module = verilogModuleModel->module();
		std::shared_ptr< lcsm::EvaluatorNode > verilogModulePhysical =
			std::make_shared< lcsm::physical::VerilogModule >(module, objectType);
		m_objects[verilogModuleId] = verilogModulePhysical;
		lcsm::support::PointerView< lcsm::EvaluatorNode > verilogModuleEvaluatorNode = verilogModulePhysical;
		lcsm::support::PointerView< lcsm::physical::VerilogModule > verilogModuleNode =
			verilogModuleEvaluatorNode.staticCast< lcsm::physical::VerilogModule >();

		// Ensure existence of inputs. Connect to VerilogModule and make them to know about each other.
		for (lcsm::portid_t portId = 0; portId < static_cast< lcsm::portid_t >(verilogModuleModel->numOfInputs()); portId++)
		{
			// Extract model.
			const lcsm::model::Wire *wireModel = verilogModuleModel->input(portId);
			const lcsm::Identifier wireId = wireModel->id();

			// Ensure existing wire.
			lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
			lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

			// Connect to each other.
			verilogModuleNode->connectInput(wireEvaluatorNode);
			wireNode->connect(verilogModuleEvaluatorNode);

			// Add wire to visit.
			queue.emplace_back(wireModel);
		}

		// Ensure existence of inputs. Connect to VerilogModule and make them to know about each other.
		for (lcsm::portid_t portId = 0; portId < static_cast< lcsm::portid_t >(verilogModuleModel->numOfInouts()); portId++)
		{
			// Extract model.
			const lcsm::model::Wire *wireModel = verilogModuleModel->inout(portId);
			const lcsm::Identifier wireId = wireModel->id();

			// Ensure existing wire.
			lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
			lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

			// Connect to each other.
			verilogModuleNode->connectInout(wireEvaluatorNode);
			wireNode->connect(verilogModuleEvaluatorNode);

			// Add wire to visit.
			queue.emplace_back(wireModel);
		}

		// Ensure existence of outputs. Connect to VerilogModule and make them to know about each other.
		for (lcsm::portid_t portId = 0; portId < static_cast< lcsm::portid_t >(verilogModuleModel->numOfOutputs()); portId++)
		{
			// Extract model.
			const lcsm::model::Wire *wireModel = verilogModuleModel->output(portId);
			const lcsm::Identifier wireId = wireModel->id();

			// Ensure existing wire.
			lcsm::support::PointerView< lcsm::EvaluatorNode > wireEvaluatorNode = registeredWire(wireId);
			lcsm::support::PointerView< lcsm::physical::Wire > wireNode = wireEvaluatorNode.staticCast< lcsm::physical::Wire >();

			// Connect to each other.
			verilogModuleNode->connectOutput(wireEvaluatorNode);
			wireNode->connect(verilogModuleEvaluatorNode);

			// Add wire to visit.
			queue.emplace_back(wireModel);
		}

		// Update visited objects.
		visited.insert(verilogModuleId);

		break;
	}
	case lcsm::ComponentType::LastCircuitType:
	{
		throw std::logic_error("Non-targeted circuit found!");
	}
	default:
	{
		throw std::logic_error("Unsupported circuit found!");
	}
	}
}
