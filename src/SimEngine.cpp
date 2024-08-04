#include <initializer_list>
#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Component.h>
#include <sim/Component/WiringComponent.h>
#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/IR/Value.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/SimCircuit.h>
#include <sim/SimContext.h>
#include <sim/SimEngine.h>
#include <sim/Support/PointerView.hpp>
#include <unordered_map>

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

sim::SimEngine::SimEngine() : m_circuits(0) {}

void sim::SimEngine::AddCircuit(sim::SimCircuit &circuit)
{
	if (m_circuits != 0)
		throw std::logic_error("");

	m_circuits++;

	for (const auto &pins : circuit.Pins())
	{
		const sim::Pin *pin = pins.second.Val().AsCircuit()->AsPin();

		if (pin->IsOutput())
			continue;

		// Create subgraph with *Pin* input.
		sim::CGPinInput *pi = RegisterPinInput(pins.first);
		sim::CG tree = sim::support::PointerView< CGObject >(pi);

		// Write value to *Pin*s wire-connector.
		sim::CGWire *pw = RegisterWire(pin->GetWire().ID());
		std::shared_ptr< sim::Instruction > wv = std::make_shared< sim::WriteValue >(pi, pw);
		sim::CGNode *I = tree.GetRoot();
		I->SetInstruction(std::move(wv));

		// **Temporary** use DFS algorithm to build Calculation Graph.
		std::unordered_map< unsigned, bool > visited;
		BuildCircuit(pin->GetWire(), pw, std::addressof(pin->GetWire()), I, visited);

		m_inputs.push_back(std::move(tree));
	}
}

std::vector< sim::Value > sim::SimEngine::InvokeFull(std::initializer_list< sim::Value > I)
{
	if (m_inputs.size() == I.size())
		throw std::logic_error("");

	return {};
}

sim::CGPinInput *sim::SimEngine::RegisterPinInput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinInput found same ID.");

	std::shared_ptr< CGObject > I = std::make_shared< sim::CGPinInput >();
	m_objects[ID] = I;

	return static_cast< sim::CGPinInput * >(I.get());
}

sim::CGPinOutput *sim::SimEngine::RegisterPinOutput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering PinOutput found same ID.");

	std::shared_ptr< CGObject > O = std::make_shared< sim::CGPinOutput >();
	m_objects[ID] = O;

	return static_cast< sim::CGPinOutput * >(O.get());
}

sim::CGWire *sim::SimEngine::RegisterWire(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found = m_objects.find(ID);

	if (found != m_objects.cend())
		throw std::logic_error("Registering Wire found same ID.");

	std::shared_ptr< CGObject > W = std::make_shared< sim::CGWire >();
	m_objects[ID] = W;

	return static_cast< sim::CGWire * >(W.get());
}

sim::CGPinInput *sim::SimEngine::RegisteredPinInput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		sim::CGObject *I = found->second.get();
		if (I && I->IsPinInput())
			return I->AsPinInput();
		else
			throw std::logic_error("RegisteredPinInput");
	}

	return RegisterPinInput(ID);
}

sim::CGPinOutput *sim::SimEngine::RegisteredPinOutput(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		CGObject *I = found->second.get();
		if (I && I->IsPinOutput())
			return I->AsPinOutput();
		else
			throw std::logic_error("RegisteredPinOutput");
	}

	return RegisterPinOutput(ID);
}

sim::CGWire *sim::SimEngine::RegisteredWire(unsigned ID)
{
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found = m_objects.find(ID);

	if (found != m_objects.cend())
	{
		CGObject *I = found->second.get();
		if (I && I->IsWire())
			return I->AsWire();
		else
			throw std::logic_error("RegisteredPinOutput");
	}

	return RegisterWire(ID);
}

void sim::SimEngine::BuildCircuit(
	const sim::Wire &current,
	sim::CGObject *targetCurrent,
	const sim::WiringComponent *parent,
	sim::CGNode *parentNode,
	std::unordered_map< unsigned, bool > &visited)
{
	std::unordered_map< unsigned, bool >::const_iterator found = visited.find(current.ID());
	if (found != visited.cend() && found->second)
		return;
	visited[current.ID()] = true;
	for (const sim::wire_t &w : current.Wires())
	{
		const sim::WiringComponent *wiringElement = w.Ptr();

		if (!wiringElement->IsWire())
			throw std::logic_error("Not implemented");

		if (wiringElement == parent)
			continue;

		const sim::Wire &wire = *wiringElement->AsWire();

		// Create *Wire* element, if not exists.
		sim::CGWire *gw = RegisteredWire(wire.ID());

		// Create *BroadcastValue* next instruction.
		std::shared_ptr< sim::Instruction > bv = std::make_shared< sim::BroadcastValue >(targetCurrent, gw);

		// Create calculation node to broadcast value from *targetRoot* to *Wire* element.
		sim::CGNode BV(sim::support::PointerView< sim::CGObject >(gw), std::move(bv));

		// Connect to parent CGNode.
		sim::CGNode *added = parentNode->AddConnection(std::move(BV));

		// Broadcast connecting.
		BuildCircuit(wire, gw, std::addressof(wire), added, visited);
	}

	for (const sim::component_t &c : current.Connections())
	{
		const sim::Component *com = c.Ptr();

		if (!com->AsCircuit())
			throw std::logic_error("Not implemented");

		const sim::CircuitComponent *circCom = com->AsCircuit();

		if (!circCom->IsPin())
			throw std::logic_error("Not implemented");

		const sim::Pin *pin = circCom->AsPin();

		if (!pin->IsOutput())
			continue;

		// Register output pin for external reading in simulation.
		sim::CGPinOutput *po = RegisteredPinOutput(pin->ID());

		// Register inout wire for output pin.
		sim::CGWire *pw = RegisteredWire(pin->GetWire().ID());

		// Write from Wire to Pin's inout wire.
		std::shared_ptr< sim::Instruction > wv = std::make_shared< sim::WriteValue >(targetCurrent, pw);
		sim::CGNode WV1(sim::support::PointerView< sim::CGObject >(pw), std::move(wv));
		sim::CGNode *writeToWire = parentNode->AddConnection(std::move(WV1));

		// Write from Pin's inout wire to Pin for external reading.
		std::shared_ptr< sim::Instruction > wvp = std::make_shared< sim::WriteValue >(pw, po);
		sim::CGNode WV2(sim::support::PointerView< sim::CGObject >(po), std::move(wvp));
		writeToWire->AddConnection(std::move(WV2));
	}
}
