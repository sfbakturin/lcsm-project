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

sim::Constant *sim::SimContext::AllocaConstant(sim::Width width, std::uint64_t value)
{
	std::shared_ptr< sim::Constant > constant = std::make_shared< sim::Constant >(width, value);
	m_allocated.push_back(constant);
	return constant.get();
}
sim::Ground *sim::SimContext::AllocaGround(sim::Width width)
{
	std::shared_ptr< sim::Ground > ground = std::make_shared< sim::Ground >(width);
	m_allocated.push_back(ground);
	return ground.get();
}
sim::Power *sim::SimContext::AllocaPower(sim::Width width)
{
	std::shared_ptr< sim::Power > power = std::make_shared< sim::Power >(width);
	m_allocated.push_back(power);
	return power.get();
}
sim::Pin *sim::SimContext::AllocaPin(bool output, sim::Width width, std::uint64_t value)
{
	std::shared_ptr< sim::Pin > pin = std::make_shared< sim::Pin >(output, width, value);
	m_allocated.push_back(pin);
	return pin.get();
}
sim::Splitter *sim::SimContext::AllocaSplitter(sim::Width widthIn, std::size_t widthOut)
{
	std::shared_ptr< sim::Splitter > spitter = std::make_shared< sim::Splitter >(widthIn, widthOut);
	m_allocated.push_back(spitter);
	return spitter.get();
}
sim::Transistor *sim::SimContext::AllocaTransistor(sim::TransistorType type)
{
	std::shared_ptr< sim::Transistor > transistor = std::make_shared< sim::Transistor >(type);
	m_allocated.push_back(transistor);
	return transistor.get();
}
sim::TransmissionGate *sim::SimContext::AllocaTransmissionGate()
{
	std::shared_ptr< sim::TransmissionGate > transmissionGate = std::make_shared< sim::TransmissionGate >();
	m_allocated.push_back(transmissionGate);
	return transmissionGate.get();
}
sim::Clock *sim::SimContext::AllocaClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	std::shared_ptr< sim::Clock > clock = std::make_shared< sim::Clock >(highDuration, lowDuration, phaseOffset);
	m_allocated.push_back(clock);
	return clock.get();
}
sim::Button *sim::SimContext::AllocaButton(bool activeOnPress)
{
	std::shared_ptr< sim::Button > button = std::make_shared< sim::Button >(activeOnPress);
	m_allocated.push_back(button);
	return button.get();
}
sim::Digit *sim::SimContext::AllocaDigit(bool hasDecimalPoint)
{
	std::shared_ptr< sim::Digit > digit = std::make_shared< sim::Digit >(hasDecimalPoint);
	m_allocated.push_back(digit);
	return digit.get();
}
sim::Probe *sim::SimContext::AllocaProbe()
{
	std::shared_ptr< sim::Probe > probe = std::make_shared< sim::Probe >();
	m_allocated.push_back(probe);
	return probe.get();
}

sim::Wire *sim::SimContext::AllocaWire()
{
	std::shared_ptr< sim::Wire > wire = std::make_shared< sim::Wire >();
	m_allocated.push_back(wire);
	return wire.get();
}

sim::Tunnel *sim::SimContext::AllocaTunnel()
{
	std::shared_ptr< sim::Tunnel > tunnel = std::make_shared< sim::Tunnel >();
	m_allocated.push_back(tunnel);
	return tunnel.get();
}
