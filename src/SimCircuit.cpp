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
#include <sim/SimCircuit.h>
#include <sim/SimContext.h>
#include <sim/Support/PointerView.hpp>

sim::SimCircuit::SimCircuit(sim::SimContext &context) : m_context(context), m_globalId(0)
{
	m_context.AddCircuit(this);
}

const std::unordered_map< unsigned, sim::support::PointerView< sim::Component > > &sim::SimCircuit::Pins() const noexcept
{
	return m_pin;
}

sim::Constant *sim::SimCircuit::RegisterConstant(sim::Width width, std::uint64_t value)
{
	sim::Constant *constant = m_context.AllocaConstant(m_globalId, width, value);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(constant);
	return constant;
}

sim::Ground *sim::SimCircuit::RegisterGround(sim::Width width)
{
	sim::Ground *ground = m_context.AllocaGround(m_globalId, width);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(ground);
	return ground;
}

sim::Power *sim::SimCircuit::RegisterPower(sim::Width width)
{
	sim::Power *power = m_context.AllocaPower(m_globalId, width);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(power);
	return power;
}

sim::Pin *sim::SimCircuit::RegisterPin(bool output, sim::Width width)
{
	sim::Pin *pin = m_context.AllocaPin(m_globalId, output, width);
	m_pin[m_globalId++] = sim::support::PointerView< sim::Component >(pin);
	pin->GetWire().Identify(m_globalId++);
	return pin;
}

sim::Splitter *sim::SimCircuit::RegisterSplitter(sim::Width widthIn, std::size_t widthOut)
{
	sim::Splitter *splitter = m_context.AllocaSplitter(m_globalId, widthIn, widthOut);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(splitter);
	return splitter;
}

sim::Transistor *sim::SimCircuit::RegisterTransistor(sim::TransistorType type)
{
	sim::Transistor *transistor = m_context.AllocaTransistor(m_globalId, type);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(transistor);
	return transistor;
}

sim::TransmissionGate *sim::SimCircuit::RegisterTransmissionGate()
{
	sim::TransmissionGate *transmissionGate = m_context.AllocaTransmissionGate(m_globalId);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(transmissionGate);
	return transmissionGate;
}

sim::Clock *sim::SimCircuit::RegisterClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	sim::Clock *clock = m_context.AllocaClock(m_globalId, highDuration, lowDuration, phaseOffset);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(clock);
	return clock;
}

sim::Button *sim::SimCircuit::RegisterButton(bool activeOnPress)
{
	sim::Button *button = m_context.AllocaButton(m_globalId, activeOnPress);
	m_io[m_globalId++] = sim::support::PointerView< sim::Component >(button);
	return button;
}

sim::Digit *sim::SimCircuit::RegisterDigit(bool hasDecimalPoint)
{
	sim::Digit *digit = m_context.AllocaDigit(m_globalId, hasDecimalPoint);
	m_io[m_globalId++] = sim::support::PointerView< sim::Component >(digit);
	return digit;
}

sim::Probe *sim::SimCircuit::RegisterProbe()
{
	sim::Probe *probe = m_context.AllocaProbe(m_globalId);
	m_io[m_globalId++] = sim::support::PointerView< sim::Component >(probe);
	return probe;
}

sim::Wire *sim::SimCircuit::RegisterWire()
{
	sim::Wire *wire = m_context.AllocaWire(m_globalId);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(wire);
	return wire;
}

sim::Tunnel *sim::SimCircuit::RegisterTunnel()
{
	sim::Tunnel *tunnel = m_context.AllocaTunnel(m_globalId);
	m_comp[m_globalId++] = sim::support::PointerView< sim::Component >(tunnel);
	return tunnel;
}
