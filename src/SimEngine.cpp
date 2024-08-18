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
#include <sim/SimState.h>
#include <sim/Support/PointerView.hpp>
#include <unordered_map>

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

sim::SimEngine::SimEngine() : m_circuits(0) {}

void sim::SimEngine::addCircuit(sim::SimCircuit &)
{
	if (m_circuits != 0)
		throw std::logic_error("");

	m_circuits++;
}

std::vector< sim::Value > sim::SimEngine::invokeFull(std::initializer_list< sim::Value >)
{
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
	std::unordered_map< unsigned, std::shared_ptr< CGObject > >::const_iterator found =
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

void sim::SimEngine::buildCircuit(
	const sim::Wire &current,
	sim::CGObject *targetCurrent,
	const sim::WiringComponent *parent,
	sim::CGNode *parentNode,
	std::unordered_map< unsigned, bool > &visited)
{
}
