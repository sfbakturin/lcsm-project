#include <sim/Component/Component.h>
#include <sim/Component/IOComponent.h>
#include <sim/Component/WiringComponent.h>
#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Clock.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Circuit/Ground.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/Model/Circuit/Power.h>
#include <sim/Model/Circuit/Splitter.h>
#include <sim/Model/Circuit/Transistor.h>
#include <sim/Model/Circuit/TransmissionGate.h>
#include <sim/Model/IO/Button.h>
#include <sim/Model/IO/Digit.h>
#include <sim/Model/IO/Probe.h>
#include <sim/Model/Wiring/Tunnel.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/SimContext.h>
#include <sim/Support/Ref.hpp>

#include <cstdint>
#include <memory>
#include <utility>

sim::SimContext::SimContext(sim::SimContext &&other) noexcept : m_allocated(std::move(other.m_allocated)) {}

sim::SimContext &sim::SimContext::operator=(sim::SimContext &&other) noexcept
{
	if (this != &other)
		sim::SimContext(std::move(other)).Swap(*this);
	return *this;
}

void sim::SimContext::Swap(sim::SimContext &other) noexcept
{
	std::swap(m_allocated, other.m_allocated);
}

void sim::SimContext::AddCircuit(sim::SimCircuit *circuit)
{
	m_circuits.push_back(sim::support::Ref< sim::SimCircuit >(circuit));
}

sim::Constant *sim::SimContext::AllocaConstant(unsigned ID, sim::Width width, std::uint64_t value)
{
	std::shared_ptr< sim::Constant > constant = std::make_shared< sim::Constant >(width, value);
	constant->Identify(ID);
	m_allocated.push_back(constant);
	return constant.get();
}
sim::Ground *sim::SimContext::AllocaGround(unsigned ID, sim::Width width)
{
	std::shared_ptr< sim::Ground > ground = std::make_shared< sim::Ground >(width);
	ground->Identify(ID);
	m_allocated.push_back(ground);
	return ground.get();
}
sim::Power *sim::SimContext::AllocaPower(unsigned ID, sim::Width width)
{
	std::shared_ptr< sim::Power > power = std::make_shared< sim::Power >(width);
	power->Identify(ID);
	m_allocated.push_back(power);
	return power.get();
}
sim::Pin *sim::SimContext::AllocaPin(unsigned ID, bool output, sim::Width width)
{
	std::shared_ptr< sim::Pin > pin = std::make_shared< sim::Pin >(output, width);
	pin->Identify(ID);
	m_allocated.push_back(pin);
	return pin.get();
}
sim::Splitter *sim::SimContext::AllocaSplitter(unsigned ID, sim::Width widthIn, std::size_t widthOut)
{
	std::shared_ptr< sim::Splitter > splitter = std::make_shared< sim::Splitter >(widthIn, widthOut);
	splitter->Identify(ID);
	m_allocated.push_back(splitter);
	return splitter.get();
}
sim::Transistor *sim::SimContext::AllocaTransistor(unsigned ID, sim::TransistorType type)
{
	std::shared_ptr< sim::Transistor > transistor = std::make_shared< sim::Transistor >(type);
	transistor->Identify(ID);
	m_allocated.push_back(transistor);
	return transistor.get();
}
sim::TransmissionGate *sim::SimContext::AllocaTransmissionGate(unsigned ID)
{
	std::shared_ptr< sim::TransmissionGate > transmissionGate = std::make_shared< sim::TransmissionGate >();
	transmissionGate->Identify(ID);
	m_allocated.push_back(transmissionGate);
	return transmissionGate.get();
}
sim::Clock *sim::SimContext::AllocaClock(unsigned ID, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	std::shared_ptr< sim::Clock > clock = std::make_shared< sim::Clock >(highDuration, lowDuration, phaseOffset);
	clock->Identify(ID);
	m_allocated.push_back(clock);
	return clock.get();
}
sim::Button *sim::SimContext::AllocaButton(unsigned ID, bool activeOnPress)
{
	std::shared_ptr< sim::Button > button = std::make_shared< sim::Button >(activeOnPress);
	button->Identify(ID);
	m_allocated.push_back(button);
	return button.get();
}
sim::Digit *sim::SimContext::AllocaDigit(unsigned ID, bool hasDecimalPoint)
{
	std::shared_ptr< sim::Digit > digit = std::make_shared< sim::Digit >(hasDecimalPoint);
	digit->Identify(ID);
	m_allocated.push_back(digit);
	return digit.get();
}
sim::Probe *sim::SimContext::AllocaProbe(unsigned ID)
{
	std::shared_ptr< sim::Probe > probe = std::make_shared< sim::Probe >();
	probe->Identify(ID);
	m_allocated.push_back(probe);
	return probe.get();
}

sim::Wire *sim::SimContext::AllocaWire(unsigned ID)
{
	std::shared_ptr< sim::Wire > wire = std::make_shared< sim::Wire >();
	wire->Identify(ID);
	m_allocated.push_back(wire);
	return wire.get();
}

sim::Tunnel *sim::SimContext::AllocaTunnel(unsigned ID)
{
	std::shared_ptr< sim::Tunnel > tunnel = std::make_shared< sim::Tunnel >();
	tunnel->Identify(ID);
	m_allocated.push_back(tunnel);
	return tunnel.get();
}
